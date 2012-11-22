using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace Server
{

	class TestProgram
	{
		static void Main()
		{
			new SGSserverForm();
		}
	}

	//The commands for interaction between the server and the client
	enum Command
	{
		Login,      //Log into the server
		Logout,     //Logout of the server
		Message,    //Send a text message to all the chat clients
		List,       //Get a list of users in the chat room from the server
		Null        //No command
	}

	class SGSserverForm
	{
		//The ClientInfo structure holds the required information about every
		//client connected to the server
		struct ClientInfo
		{
			public EndPoint endpoint;   //Socket of the client
			public string strName;      //Name by which the user logged into the chat room
		}

		//The collection of all clients logged into the room (an array of type ClientInfo)
		public static ArrayList clientList;

		//The main socket on which the server listens to the clients
		Socket serverSocket;

		byte[] byteData = new byte[1024];
		short UdpPort = 3000;

		static ManualResetEvent resetEvent = new ManualResetEvent(false);

		public void Start()
		{
			try
			{
				//We are using UDP sockets
				serverSocket = new Socket(AddressFamily.InterNetwork,
					SocketType.Dgram, ProtocolType.Udp);

				var localIP = getLocalHosts();

				Console.WriteLine(" >> SERVER INFO << IP: {0}, UDP port: {1}", localIP.ToString(), UdpPort);

				//Assign the any IP of the machine and listen on port number 1000
				IPEndPoint ipEndPoint = new IPEndPoint(IPAddress.Any, UdpPort);

				//Bind this address to the server
				serverSocket.Bind(ipEndPoint);

				IPEndPoint ipeSender = new IPEndPoint(IPAddress.Any, 0);
				//The epSender identifies the incoming clients
				EndPoint epSender = (EndPoint)ipeSender;

				//Start receiving data
				serverSocket.BeginReceiveFrom(byteData, 0, byteData.Length,
					SocketFlags.None, ref epSender, new AsyncCallback(OnReceive), epSender);
				
				resetEvent.WaitOne();
			}
			catch (Exception ex)
			{
				Console.WriteLine("SGSServerUDP | " + ex.Message);
			}
		}

		public SGSserverForm()
		{
			clientList = new ArrayList();
			Start();
		}

		private void OnReceive(IAsyncResult ar)
		{
			try
			{
				IPEndPoint ipeSender = new IPEndPoint(IPAddress.Any, 0);
				EndPoint epSender = (EndPoint)ipeSender;

				serverSocket.EndReceiveFrom(ar, ref epSender);

				//Transform the array of bytes received from the user into an
				//intelligent form of object Data
				Data msgReceived = new Data(byteData);

				//We will send this object in response the users request
				Data msgToSend = new Data();

				byte[] message;

				//If the message is to login, logout, or simple text message
				//then when send to others the type of the message remains the same
				msgToSend.cmdCommand = msgReceived.cmdCommand;
				msgToSend.strName = msgReceived.strName;

				switch (msgReceived.cmdCommand)
				{
					case Command.Login:

						//When a user logs in to the server then we add her to our
						//list of clients

						ClientInfo clientInfo = new ClientInfo();
						clientInfo.endpoint = epSender;
						clientInfo.strName = msgReceived.strName;

						clientList.Add(clientInfo);

						//Set the text of the message that we will broadcast to all users
						msgToSend.strMessage = "<<<" + msgReceived.strName + " has joined the room>>>";
						break;

					case Command.Logout:

						//When a user wants to log out of the server then we search for her 
						//in the list of clients and close the corresponding connection

						int nIndex = 0;
						foreach (ClientInfo client in clientList)
						{
							if (client.endpoint == epSender)
							{
								clientList.RemoveAt(nIndex);
								break;
							}
							++nIndex;
						}

						msgToSend.strMessage = "<<<" + msgReceived.strName + " has left the room>>>";
						break;

					case Command.Message:

						//Set the text of the message that we will broadcast to all users
						msgToSend.strMessage = msgReceived.strName + ": " + msgReceived.strMessage;
						break;

					case Command.List:

						//Send the names of all users in the chat room to the new user
						msgToSend.cmdCommand = Command.List;
						msgToSend.strName = null;
						msgToSend.strMessage = null;

						//Collect the names of the user in the chat room
						foreach (ClientInfo client in clientList)
						{
							//To keep things simple we use asterisk as the marker to separate the user names
							msgToSend.strMessage += client.strName + "*";
						}

						message = msgToSend.ToByte();

						//Send the name of the users in the chat room
						serverSocket.BeginSendTo(message, 0, message.Length, SocketFlags.None, epSender,
								new AsyncCallback(OnSend), epSender);
						break;
				}

				if (msgToSend.cmdCommand != Command.List)   //List messages are not broadcasted
				{
					message = msgToSend.ToByte();

					foreach (ClientInfo clientInfo in clientList)
					{
						if (clientInfo.endpoint != epSender ||
							msgToSend.cmdCommand != Command.Login)
						{
							//Send the message to all users
							serverSocket.BeginSendTo(message, 0, message.Length, SocketFlags.None, clientInfo.endpoint,
								new AsyncCallback(OnSend), clientInfo.endpoint);
						}
					}

					Console.WriteLine(msgToSend.strMessage);
				}

				//If the user is logging out then we need not listen from her
				if (msgReceived.cmdCommand != Command.Logout)
				{
					//Start listening to the message send by the user
					serverSocket.BeginReceiveFrom(byteData, 0, byteData.Length, SocketFlags.None, ref epSender,
						new AsyncCallback(OnReceive), epSender);
				}
			}
			catch (Exception ex)
			{
				Console.WriteLine("SGSServerUDP | " + ex.Message);
			}
		}

		public void OnSend(IAsyncResult ar)
		{
			try
			{
				serverSocket.EndSend(ar);
			}
			catch (Exception ex)
			{
				Console.WriteLine("SGSServerUDP | " + ex.Message);
			}
		}

		private IPAddress getLocalHosts()
		{
			IPHostEntry localHostEntry;
			IPAddress localIP = null;

			try
			{
				localHostEntry = Dns.GetHostEntry(Dns.GetHostName());
			}
			catch (Exception)
			{
				Console.WriteLine("Local Host not found"); // fail
				return null;
			}

			foreach (var ip in localHostEntry.AddressList)
			{
				if (ip.AddressFamily == AddressFamily.InterNetwork)
				{
					localIP = ip;
				}
			}

			if (localIP == null)
				Console.WriteLine("-- BŁĄD -- getLocalHosts -> IP address is empty!");

			return localIP;
		}
	}

	//The data structure by which the server and the client interact with 
	//each other
	class Data
	{
		//Default constructor
		public Data()
		{
			this.cmdCommand = Command.Null;
			this.strMessage = null;
			this.strName = null;
		}

		//Converts the bytes into an object of type Data
		public Data(byte[] data)
		{
			//The first four bytes are for the Command
			this.cmdCommand = (Command)BitConverter.ToInt32(data, 0);

			//The next four store the length of the name
			int nameLen = BitConverter.ToInt32(data, 4);

			//The next four store the length of the message
			int msgLen = BitConverter.ToInt32(data, 8);

			//This check makes sure that strName has been passed in the array of bytes
			if (nameLen > 0)
				this.strName = Encoding.UTF8.GetString(data, 12, nameLen);
			else
				this.strName = null;

			//This checks for a null message field
			if (msgLen > 0)
				this.strMessage = Encoding.UTF8.GetString(data, 12 + nameLen, msgLen);
			else
				this.strMessage = null;
		}

		//Converts the Data structure into an array of bytes
		public byte[] ToByte()
		{
			List<byte> result = new List<byte>();

			//First four are for the Command
			result.AddRange(BitConverter.GetBytes((int)cmdCommand));

			//Add the length of the name
			if (strName != null)
				result.AddRange(BitConverter.GetBytes(strName.Length));
			else
				result.AddRange(BitConverter.GetBytes(0));

			//Length of the message
			if (strMessage != null)
				result.AddRange(BitConverter.GetBytes(strMessage.Length));
			else
				result.AddRange(BitConverter.GetBytes(0));

			//Add the name
			if (strName != null)
				result.AddRange(Encoding.UTF8.GetBytes(strName));

			//And, lastly we add the message text to our array of bytes
			if (strMessage != null)
				result.AddRange(Encoding.UTF8.GetBytes(strMessage));

			return result.ToArray();
		}

		public string strName;      //Name by which the client logs into the room
		public string strMessage;   //Message text
		public Command cmdCommand;  //Command type (login, logout, send message, etcetera)
	}
}

//namespace AirPlanesServer
//{
//	public enum Command
//	{
//		Login,      //Log into the server
//		Logout,     //Logout of the server
//		Message,    //Send a text message to all the chat clients
//		List,       //Get a list of users in the chat room from the server
//		Null        //No command
//	}

//	class UdpServer
//	{

//		//The ClientInfo structure holds the required information about every
//		//client connected to the server
//		struct ClientInfo
//		{
//			public EndPoint endpoint;   //Socket of the client
//			public string strName;      //Name by which the user logged into the chat room
//		}

//		static void Main()
//		{
//			new UdpServer();
//		}
		
//		private const short UdpPort = 3000;

//		private ArrayList _clientList;				//The collection of all clients logged into the room (an array of type ClientInfo)
//		private Socket _socketUdp;					//The main socket on which the server listens to the clients
//		private byte[] byteData = new byte[1024];

//		private static readonly ManualResetEvent ResetEvent = new ManualResetEvent(false);

//		public UdpServer()
//		{
//			try
//			{
//				_clientList = new ArrayList();
//				ReceiveAndSendBack();
//				Console.WriteLine("Started Server / UDP Sender-Receiver Thread!\n");
//				ResetEvent.WaitOne(); // Blocks until "set"
//			}
//			catch (Exception e)
//			{
//				Console.WriteLine("An UDP Exception has occurred!" + e.ToString());
//			}
//		}

//		private IPAddress getLocalHosts()
//		{
//			IPHostEntry localHostEntry;
//			IPAddress localIP = null;

//			try
//			{
//				localHostEntry = Dns.GetHostEntry(Dns.GetHostName());
//			}
//			catch (Exception)
//			{
//				Console.WriteLine("Local Host not found"); // fail
//				return null;
//			}

//			foreach (var ip in localHostEntry.AddressList)
//			{
//				if (ip.AddressFamily == AddressFamily.InterNetwork)
//				{
//					localIP = ip;
//				}
//			}

//			if (localIP == null)
//				Console.WriteLine("-- BŁĄD -- getLocalHosts -> IP address is empty!");

//			return localIP;
//		}

//		public void ReceiveAndSendBack()
//		{
//			try
//			{
//				//Create a UDP socket.
//				_socketUdp = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
//				var localIP = getLocalHosts();

//				Console.WriteLine(" >> SERVER INFO << IP: {0}, UDP port: {1}",localIP.ToString(), UdpPort);
//				//Assign the any IP of the machine and listen on port
//				var localIpEndPoint = new IPEndPoint(IPAddress.Any, 3000);
				
//				//Bind this address to the server
//				_socketUdp.Bind(localIpEndPoint);

//				IPEndPoint ipSender = new IPEndPoint(IPAddress.Any, 0);
//				//The epSender identifies the incoming clients
//				EndPoint endPointSender = (EndPoint)ipSender;

//				//Start receiving data
//				_socketUdp.BeginReceiveFrom(byteData, 0, byteData.Length,
//					SocketFlags.None, ref endPointSender, new AsyncCallback(OnReceive), endPointSender); 
//			}
//			catch (Exception se)
//			{
//				Console.WriteLine("A Exception has occurred!" + se.ToString());
//			}
//		}

//		private void OnReceive(IAsyncResult ar)
//		{
//			IPEndPoint ipeSender = new IPEndPoint(IPAddress.Any, 0);
//			EndPoint epSender = (EndPoint)ipeSender;

//			_socketUdp.EndReceiveFrom(ar, ref epSender);

//			//Transform the array of bytes received from the user into an
//			//intelligent form of object Data
//			var msgReceived = new Data(byteData);

//			//We will send this object in response the users request
//			var msgToSend = new Data();

//			//If the message is to login, logout, or simple text message
//			//then when send to others the type of the message remains the same
//			msgToSend.cmdCommand = msgReceived.cmdCommand;
//			msgToSend.strName = msgReceived.strName;

//			byte[] message;

//			switch (msgReceived.cmdCommand)
//			{
//				case Command.Login:

//					//When a user logs in to the server then we add her to our
//					//list of clients

//					ClientInfo clientInfo = new ClientInfo();
//					clientInfo.endpoint = epSender;
//					clientInfo.strName = msgReceived.strName;

//					_clientList.Add(clientInfo);

//					//Set the text of the message that we will broadcast to all users
//					msgToSend.strMessage = "<<<" + msgReceived.strName + " has joined the room>>>";
//					Console.WriteLine(msgToSend.strMessage);
//					break;

//				case Command.Logout:

//					//When a user wants to log out of the server then we search for her 
//					//in the list of clients and close the corresponding connection

//					int nIndex = 0;
//					foreach (ClientInfo client in _clientList)
//					{
//						if (client.endpoint == epSender)
//						{
//							_clientList.RemoveAt(nIndex);
//							break;
//						}
//						++nIndex;
//					}

//					msgToSend.strMessage = "<<<" + msgReceived.strName + " has left the room>>>";
//					Console.WriteLine(msgToSend.strMessage);
//					break;

//				case Command.Message:

//					//Set the text of the message that we will broadcast to all users
//					msgToSend.strMessage = msgReceived.strName + ": " + msgReceived.strMessage;
//					Console.WriteLine(msgToSend.strMessage);
//					break;

//				case Command.List:

//					//Send the names of all users in the chat room to the new user
//					msgToSend.cmdCommand = Command.List;
//					msgToSend.strName = null;
//					msgToSend.strMessage = null;

//					//Collect the names of the user in the chat room
//					foreach (ClientInfo client in _clientList)
//					{
//						//To keep things simple we use asterisk as the marker to separate the user names
//						msgToSend.strMessage += client.strName + "*";
//					}

//					message = msgToSend.ToByte();

//					//Send the name of the users in the chat room
//					_socketUdp.BeginSendTo(message, 0, message.Length, SocketFlags.None, epSender,
//							new AsyncCallback(OnSend), epSender);
//					break;
//			}

//			if (msgReceived.strMessage == "serverstop")
//				ResetEvent.Set(); // Allow the program to exit
//		}

//		public void OnSend(IAsyncResult ar)
//		{
//			try
//			{
//				_socketUdp.EndSend(ar);
//			}
//			catch (Exception ex)
//			{
//				Console.WriteLine("SGSServerUDP | " + ex.Message );
//			}
//		}
//	}


//	//The data structure by which the server and the client interact with 
//	//each other
//	public class Data
//	{
//		//Default constructor
//		public Data()
//		{
//			this.cmdCommand = Command.Null;
//			this.strMessage = null;
//			this.strName = null;
//		}

//		//Converts the bytes into an object of type Data
//		public Data(byte[] data)
//		{
//			//The first four bytes are for the Command
//			this.cmdCommand = (Command)BitConverter.ToInt32(data, 0);

//			//The next four store the length of the name
//			var nameLen = BitConverter.ToInt32(data, 4);

//			//The next four store the length of the message
//			var msgLen = BitConverter.ToInt32(data, 8);

//			//This check makes sure that strName has been passed in the array of bytes
//			if (nameLen > 0)
//				this.strName = Encoding.UTF8.GetString(data, 12, nameLen);
//			else
//				this.strName = null;

//			//This checks for a null message field
//			if (msgLen > 0)
//				this.strMessage = Encoding.UTF8.GetString(data, 12 + nameLen, msgLen);
//			else
//				this.strMessage = null;
//		}

//		//Converts the Data structure into an array of bytes
//		public byte[] ToByte()
//		{
//			var result = new List<byte>();

//			//First four are for the Command
//			result.AddRange(BitConverter.GetBytes((int)cmdCommand));

//			//Add the length of the name
//			if (strName != null)
//				result.AddRange(BitConverter.GetBytes(strName.Length));
//			else
//				result.AddRange(BitConverter.GetBytes(0));

//			//Length of the message
//			if (strMessage != null)
//				result.AddRange(BitConverter.GetBytes(strMessage.Length));
//			else
//				result.AddRange(BitConverter.GetBytes(0));

//			//Add the name
//			if (strName != null)
//				result.AddRange(Encoding.UTF8.GetBytes(strName));

//			//And, lastly we add the message text to our array of bytes
//			if (strMessage != null)
//				result.AddRange(Encoding.UTF8.GetBytes(strMessage));

//			return result.ToArray();
//		}

//		public string strName { get; set; }      //Name by which the client logs into the room
//		public string strMessage { get; set; }   //Message text
//		public Command cmdCommand { get; set; }  //Command type (id, send message, etcetera)
//	}
//}
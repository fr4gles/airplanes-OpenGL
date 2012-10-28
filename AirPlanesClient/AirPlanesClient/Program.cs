using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace AirPlanesClient
{
	class MyUdpClient
	{
		static void Main()
		{
			//Console.WriteLine("Usage: sampleTcpUdpClient2 <TCP or UDP> <Server Name or IP Address> Message");
			//Console.WriteLine("Example: sampleTcpUdpClient2 TCP localhost ''hello. how are you?''");
			
			
			Console.WriteLine("Wprowadź adres ip serwera: (pozostaw puste jeśli adres ip to 192.168.0.100)");
			var ip = Console.ReadLine();
			if (ip == "") ip = "192.168.0.100";
			
			Console.Clear();
			Console.WriteLine("Połączenie z serwerem : {0}, port: {1}",ip,UdPport);

			var client = new MyUdpClient(ip);
			
			while (true)
			{
				Console.Write("Wiadomosc (q = quit): ");
				var msg = Console.ReadLine();
				if (msg == "q") break;
				client.SendAndPrintRecievedMsg(msg);
			}
			client.CloseUdpClient();
			Console.WriteLine("The UDP server is disconnected.");
		}

		private const short UdPport = 3000;
		private UdpClient	_udpClient;
		private IPEndPoint	_remoteIpEndPoint;
		private EndPoint	_localIPEndPoint;
		private IPAddress	_remoteHostEntry;

		public MyUdpClient(string ip)
		{
			InitializeUdpClient(ip);
		}

		private static IPAddress GetLocalHosts()
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

		private void InitializeUdpClient(string serverName)
		{
			try
			{
				_udpClient = new UdpClient(serverName, UdPport);
				_localIPEndPoint = _udpClient.Client.LocalEndPoint;
				_remoteHostEntry = GetLocalHosts();
				_remoteIpEndPoint = new IPEndPoint(_remoteHostEntry, UdPport);

				Console.WriteLine(" IP klienta: {0} , port : {1}", _remoteHostEntry, _localIPEndPoint);
			}
			catch (Exception e)
			{
				Console.WriteLine(" -- An Exception Occurred! -- InitializeUdpClient  | " + e.ToString());
			}
		}

		private void SendAndPrintRecievedMsg(string msg)
		{
			try
			{
				var inputToBeSent = Encoding.ASCII.GetBytes(msg.ToCharArray());
				var nBytesSent = _udpClient.Send(inputToBeSent, inputToBeSent.Length);
				var received = _udpClient.Receive(ref _remoteIpEndPoint);
				var dataReceived = Encoding.ASCII.GetString(received);
				Console.WriteLine(dataReceived);
			}
			catch (Exception e)
			{
				Console.WriteLine(" -- An Exception Occurred! -- SendAndPrintRecievedMsg  | " + e.ToString());
			}
		}

		private void CloseUdpClient()
		{
			try
			{
				_udpClient.Close();
			}
			catch (Exception e)
			{
				Console.WriteLine(" -- An Exception Occurred! -- CloseUdpClient  | " + e.ToString());
			}
		}
	}
}


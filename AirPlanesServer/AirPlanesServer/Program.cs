using System;
using System.Globalization;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace AirPlanesServer
{
	class UdpServer
	{
		static void Main(string[] args)
		{
			new UdpServer();
		}

		private const short UdpPort = 3000;
		public Thread Udpthread;

		public UdpServer()
		{
			try
			{
				//Starting the UDP Server thread.
				Udpthread = new Thread(new ThreadStart(StartReceiveFrom2));
				Udpthread.Start();
				Console.WriteLine("Started Server / UDP Sender-Receiver Thread!\n");
			}
			catch (Exception e)
			{
				Console.WriteLine("An UDP Exception has occurred!" + e.ToString());
				Udpthread.Abort();
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

		public void StartReceiveFrom2()
		{
			try
			{
				//Create a UDP socket.
				var soUdp = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
				var localIP = getLocalHosts();

				Console.WriteLine(" >> SERVER INFO << IP: {0}, UDP port: {1}",localIP.ToString(), UdpPort);
				var localIpEndPoint = new IPEndPoint(localIP, UdpPort);
				soUdp.Bind(localIpEndPoint);
				
				while (true)
				{
					var received = new Byte[256];
					var tmpIpEndPoint = new IPEndPoint(localIP, UdpPort);
					EndPoint remoteEP = (tmpIpEndPoint);
					var bytesReceived = soUdp.ReceiveFrom(received, ref remoteEP);
					var dataReceived = System.Text.Encoding.ASCII.GetString(received);
					Console.WriteLine("SampleClient is connected through UDP. | {0}",remoteEP.ToString());
					Console.WriteLine(dataReceived);
					var returningString = "The Server got your message through UDP:" + dataReceived.ToString(CultureInfo.InvariantCulture);
					var returningByte = System.Text.Encoding.ASCII.GetBytes(returningString.ToCharArray());
					soUdp.SendTo(returningByte, remoteEP);
				}
			}
			catch (SocketException se)
			{
				Console.WriteLine("A Socket Exception has occurred!" + se.ToString());
			}
		}
	}
}
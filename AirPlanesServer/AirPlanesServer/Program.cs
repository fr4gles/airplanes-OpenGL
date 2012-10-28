using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Net;
using System.Net.Sockets;

namespace AirPlanesServer
{
	class UDPServer
	{
		static void Main(string[] args)
		{
			UDPServer game_server = new UDPServer(); 
		}

		private const short _udpPort = 3000;
		public Thread sampleUdpThread;

		public UDPServer()
		{
			try
			{
				//Starting the UDP Server thread.
				sampleUdpThread = new Thread(new ThreadStart(StartReceiveFrom2));
				sampleUdpThread.Start();
				Console.WriteLine("Started SampleTcpUdpServer's UDP Receiver Thread!\n");
			}
			catch (Exception e)
			{
				Console.WriteLine("An UDP Exception has occurred!" + e.ToString());
				sampleUdpThread.Abort();
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

			foreach (IPAddress ip in localHostEntry.AddressList)
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
				Socket soUdp = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
				IPAddress localIP = getLocalHosts();

				Console.WriteLine(" >> SERVER INFO << IP: {0}, UDP port: {1}",localIP.ToString(), _udpPort);
				IPEndPoint localIpEndPoint = new IPEndPoint(localIP, _udpPort);
				soUdp.Bind(localIpEndPoint);
				
				while (true)
				{
					Byte[] received = new Byte[256];
					IPEndPoint tmpIpEndPoint = new IPEndPoint(localIP, _udpPort);
					EndPoint remoteEP = (tmpIpEndPoint);
					int bytesReceived = soUdp.ReceiveFrom(received, ref remoteEP);
					String dataReceived = System.Text.Encoding.ASCII.GetString(received);
					Console.WriteLine("SampleClient is connected through UDP.");
					Console.WriteLine(dataReceived);
					String returningString = "The Server got your message through UDP:" + dataReceived;
					Byte[] returningByte = System.Text.Encoding.ASCII.GetBytes(returningString.ToCharArray());
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
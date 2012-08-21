using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

public class nsWinSockSharp
{
    /// <summary>
    /// Interface to ws2_32.dll
    /// </summary>
    public unsafe partial class Native
    {
        public const int SOCKET_ERROR = -1;
        public const int INVALID_SOCKET = ~0;

        [DllImport("Ws2_32.dll")]
        public static extern int WSAStartup(ushort Version, out WSAData Data);

        [DllImport("Ws2_32.dll")]
        public static extern System.Net.Sockets.SocketError WSAGetLastError();

        [DllImport("Ws2_32.dll")]
        public static extern SOCKET socket(AddressFamily af, SocketType type, ProtocolType protocol);

        [DllImport("Ws2_32.dll")]
        public static extern int send(SOCKET s, byte* buf, int len, int flags);

        [DllImport("Ws2_32.dll")]
        public static extern int recv(SOCKET s, byte* buf, int len, int flags);

        [DllImport("Ws2_32.dll")]
        public static extern SOCKET accept(SOCKET s, void* addr, int addrsize);

        [DllImport("Ws2_32.dll")]
        public static extern int listen(SOCKET s, int backlog);

        [DllImport("Ws2_32.dll", CharSet = CharSet.Ansi)]
        public static extern uint inet_addr(string cp);

        [DllImport("Ws2_32.dll")]
        public static extern ushort htons(ushort hostshort);

        [DllImport("Ws2_32.dll")]
        public static extern int connect(SOCKET s, sockaddr_in* addr, int addrsize);

        [DllImport("Ws2_32.dll")]
        public static extern int closesocket(SOCKET s);

        [DllImport("Ws2_32.dll")]
        public static extern int getpeername(SOCKET s, sockaddr_in* addr, int* addrsize);

        [DllImport("Ws2_32.dll")]
        public static extern int bind(SOCKET s, sockaddr_in* addr, int addrsize);

        [DllImport("Ws2_32.dll")]
        public static extern int select(int ndfs, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, timeval* timeout);

        [DllImport("Ws2_32.dll")]
        public static extern sbyte* inet_ntoa(sockaddr_in.in_addr _in);
    }

    // enum from Microsoft website (#defined in C++)
    public enum AddressFamily : int
    {
        Unknown = 0,
        InterNetworkv4 = 2,
        Ipx = 4,
        AppleTalk = 17,
        NetBios = 17,
        InterNetworkv6 = 23,
        Irda = 26,
        BlueTooth = 32
    }
    public enum SocketType : int
    {
        Unknown = 0,
        Stream = 1,
        DGram = 2,
        Raw = 3,
        Rdm = 4,
        SeqPacket = 5
    }
    public enum ProtocolType : int
    {
        BlueTooth = 3,
        Tcp = 6,
        Udp = 17,
        ReliableMulticast = 113
    }

    /// <summary>
    ///  equivalent to C++'s "SOCKET"
    /// </summary>
    public unsafe struct SOCKET
    {
        private void* handle;
        private SOCKET(int _handle)
        {
            handle = (void*)_handle;
        }
        public static bool operator ==(SOCKET s, int i)
        {
            return ((int)s.handle == i);
        }
        public static bool operator !=(SOCKET s, int i)
        {
            return ((int)s.handle != i);
        }
        public static implicit operator SOCKET(int i)
        {
            return new SOCKET(i);
        }
        public static implicit operator uint(SOCKET s)
        {
            return (uint)s.handle;
        }
        public override bool Equals(object obj)
        {
            return (obj is SOCKET) ? (((SOCKET)obj).handle == this.handle) : base.Equals(obj);
        }
        public override int GetHashCode()
        {
            return (int)handle;
        }
    }

    /// <summary>
    /// fd_set used in 'select' method
    /// </summary>
    public unsafe struct fd_set
    {
        public const int FD_SETSIZE = 64;
        public uint fd_count;
        public fixed uint fd_array[FD_SETSIZE];
    }

    /// <summary>
    /// C# equivalent to C++'s sockaddr_in / SOCKADDR_IN
    /// </summary>
    [StructLayout(LayoutKind.Sequential, Size = 16)]
    public struct sockaddr_in
    {
        public const int Size = 16;

        public short sin_family;
        public ushort sin_port;
        public struct in_addr
        {
            public uint S_addr;
            public struct _S_un_b
            {
                public byte s_b1, s_b2, s_b3, s_b4;
            }
            public _S_un_b S_un_b;
            public struct _S_un_w
            {
                public ushort s_w1, s_w2;
            }
            public _S_un_w S_un_w;
        }
        public in_addr sin_addr;
    }

    /// <summary>
    ///  WSAData structure, used in WSAStarutp
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public unsafe struct WSAData
    {
        public ushort Version;
        public ushort HighVersion;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 257)]
        public string Description;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 129)]
        public string SystemStatus;
        public ushort MaxSockets;
        public ushort MaxUdpDg;
        sbyte* lpVendorInfo;
    }
}

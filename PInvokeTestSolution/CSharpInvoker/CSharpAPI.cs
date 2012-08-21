using System;
using System.Net.Sockets;
using nsWinSockSharp;
using System.Text;
using System.Runtime.InteropServices;

public unsafe class CSharpAPI
{
    [DllImport("CPlusAPI.dll", EntryPoint="GetDataInt")] public static extern int GetDataInt1(int* data_ptr);
    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")] public static extern int GetDataInt2(ref int data_ptr);
    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")] public static extern int GetDataInt3(out int data_ptr);

    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataIntRef1(int* data);
    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataIntRef2(ref int data);
    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataIntRef3(out int data);

    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataDouble1(double* data_ptr);
    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataDouble2(ref double data_ptr);
    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataDouble3(out double data_ptr);

    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataDoubleRef1(double* data);
    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataDoubleRef2(ref double data);
    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataDoubleRef3(out double data);


    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataSockaddr1(nsWinSockSharp.sockaddr_in* data_ptr);
    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataSockaddr2(ref nsWinSockSharp.sockaddr_in data_ptr);
    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataSockaddr3(out nsWinSockSharp.sockaddr_in data_ptr);

    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataSockaddrRef1(nsWinSockSharp.sockaddr_in* data);
    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataSockaddrRef2(ref nsWinSockSharp.sockaddr_in data);
    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataSockaddrRef3(out nsWinSockSharp.sockaddr_in data);

    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataWchar(char* data_ptr);
    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataWcharRef(char* data_ptr);

    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataChar(char* data_ptr);
    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataCharEx(char* data);

    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataStr1(char* data_ptr);
    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataStr2(ref char[] data_ptr);
    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataStr3(StringBuilder data_ptr);

    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataStrRef1(char* data_ptr);
    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataStrRef2(ref char[] data_ptr);
    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataStrRef3(StringBuilder data_ptr);

    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataWstr1(char* data_ptr);
    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataWstr2(ref char[] data_ptr);
    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataWstr3(StringBuilder data_ptr);

    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataWstrEx1(char* data);
    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataWstrEx2(ref char[] data);
    [DllImport("CPlusAPI.dll", EntryPoint = "GetDataInt")]
    public static extern int GetDataWstrEx3(StringBuilder data);
}
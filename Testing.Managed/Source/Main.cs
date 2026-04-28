using System;
using System.Runtime.InteropServices;

using Rolky;
using Rolky.Interop;

namespace Testing
{

    public class Test
    {
        [UnmanagedCallersOnly]
        public static void TestMain(UnmanagedString InString)
        {
            Console.WriteLine(InString);
        }
    }

}
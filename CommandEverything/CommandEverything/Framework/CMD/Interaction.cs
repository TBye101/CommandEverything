using CommandEverything.Framework.Util.Text;
using System;
using System.Diagnostics;
using System.IO;

namespace CommandEverything.Framework.CMD
{
    /// <summary>
    /// This class handles all interaction between this program and the system CMD.
    /// </summary>
    public class Interaction
    {
        /// <summary>
        /// Launches a CMD window.
        /// </summary>
        /// <param name="Command"></param>
        public static void NewCMD()
        {
            ProcessStartInfo startInfo = new ProcessStartInfo("cmd")
            {
                WindowStyle = ProcessWindowStyle.Normal,
                UseShellExecute = true,
                RedirectStandardOutput = false,
                CreateNoWindow = true
            };

            Process process = Process.Start(startInfo);
        }
    }
}
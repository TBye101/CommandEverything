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
        public static void LaunchCmdCommand(string Command)
        {
            ProcessStartInfo startInfo = new ProcessStartInfo("cmd", Command)
            {
                WindowStyle = ProcessWindowStyle.Normal,
                UseShellExecute = false,
                RedirectStandardOutput = false,
                CreateNoWindow = false
            };

            Process process = Process.Start(startInfo);
            process.OutputDataReceived += (sender, e) => ConsoleWriter.WriteLine(e.Data);
            process.BeginOutputReadLine();
            process.Start();
            
            //Need to make this run concurrently and clean up afterwards.
        }

        private static void p_ErrorDataReceived(object sender, DataReceivedEventArgs e)
        {
            Process p = sender as Process;
            if (p == null)
                return;
            Console.WriteLine(e.Data);
        }

        private static void p_OutputDataReceived(object sender, DataReceivedEventArgs e)
        {
            Process p = sender as Process;
            if (p == null)
                return;
            Console.WriteLine(e.Data);
        }
    }
}
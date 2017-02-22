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
        public void LaunchCmdCommand(string Command)
        {
            string output = string.Empty;
            string error = string.Empty;

            ProcessStartInfo processStartInfo = new ProcessStartInfo("cmd", Command);
            processStartInfo.RedirectStandardOutput = true;
            processStartInfo.RedirectStandardError = true;
            processStartInfo.WindowStyle = ProcessWindowStyle.Normal;
            processStartInfo.UseShellExecute = false;

            Process process = Process.Start(processStartInfo);
            using (StreamReader streamReader = process.StandardOutput)
            {
                output = streamReader.ReadToEnd();
            }

            using (StreamReader streamReader = process.StandardError)
            {
                error = streamReader.ReadToEnd();
            }

            ConsoleWriter.WriteLine(output, "CMD");

            if (!string.IsNullOrEmpty(error))
            {
                ConsoleWriter.WriteLine("The following error was detected:");
                ConsoleWriter.WriteLine(error, "CMD");
            }
        }
    }
}
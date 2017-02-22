using CommandEverything.Framework.Util;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CommandEverything.Framework.CMD
{
    /// <summary>
    /// This class handles all interaction between this program and the system CMD.
    /// </summary>
    public class Interaction
    {
        public void Go()
        {
            string output = string.Empty;
            string error = string.Empty;

            ProcessStartInfo processStartInfo = new ProcessStartInfo("cmd", "/c dir");
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

            ConsoleWriter.WriteLine("The following output was detected:");
            ConsoleWriter.WriteLine(output);

            if (!string.IsNullOrEmpty(error))
            {
                ConsoleWriter.WriteLine("The following error was detected:");
                ConsoleWriter.WriteLine(error);
            }

            Console.Read();
        }
    }
}

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace CommandEverything.Framework.Util.Text
{
    /// <summary>
    /// Has some code for logging.
    /// </summary>
    public static class Logging
    {
        private static string LogFilePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + "\\Logs\\" + Path.GetRandomFileName() + ".txt";
        private static StreamWriter a = CreateStreamWriter(LogFilePath);

        /// <summary>
        /// Creates a streamwriter object.
        /// </summary>
        /// <param name="Path"></param>
        /// <returns></returns>
        private static StreamWriter CreateStreamWriter(string Path)
        {
            StreamWriter Stream = File.CreateText(Path);
            Stream.AutoFlush = true;

            return Stream;
        }

        /// <summary>
        /// Actually Logs a message.
        /// </summary>
        /// <param name="ToLog"></param>
        public static void Log(string ToLog)
        {
            a.WriteLine(ToLog);
        }
    }
}
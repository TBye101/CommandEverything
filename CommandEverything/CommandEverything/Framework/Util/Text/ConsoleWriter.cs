using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CommandEverything.Framework.Util.Text
{
    /// <summary>
    /// Knows how to do more advanced kinds of output to the console.
    /// </summary>
    public class ConsoleWriter
    {
        private static string Time
        {
            get
            {
                return "[" + DateTime.Now.ToString() + "]";
            }
        }

        /// <summary>
        /// Writes the message to the console.
        /// </summary>
        /// <param name="ToWrite">The message</param>
        /// <param name="Program">Name of the program this message should be displayed as coming from.</param>
        public static void WriteLine(Writable ToWrite, string Program)
        {
            ConsoleColor Old = Console.ForegroundColor;
            Console.ForegroundColor = ToWrite.Color;
            BasicWriteLine("______________________________________________________");
            BasicWriteLine(Time + "(" + Program + ")");
            BasicWriteLine(Time + " " + ToWrite.Message);
            BasicWriteLine("______________________________________________________");
            Console.ForegroundColor = Old;
        }

        /// <summary>
        /// Writes the line to the console and logs it.
        /// </summary>
        /// <param name="ToWrite"></param>
        private static void BasicWriteLine(string ToWrite)
        {
            Logging.Log(ToWrite);
            Console.WriteLine(ToWrite);
        }

        /// <summary>
        /// Same thing as the other Writeline, except this assumes this is internal and doesn't display what program it is from.
        /// </summary>
        /// <param name="ToWrite"></param>
        public static void WriteLine(Writable ToWrite)
        {
            ConsoleColor Old = Console.ForegroundColor;
            Console.ForegroundColor = ToWrite.Color;
            BasicWriteLine(Time + " " + ToWrite.Message);
            Console.ForegroundColor = Old;
        }

        public static void WriteLine(string ToWrite)
        {
            BasicWriteLine(Time + " " + ToWrite);
        }

        public static void WriteLine(string ToWrite, string Program)
        {
            BasicWriteLine("______________________________________________________");
            BasicWriteLine(Time + "(" + Program + ")");
            BasicWriteLine(Time + " " + ToWrite);
            BasicWriteLine("______________________________________________________");
        }

        /// <summary>
        /// Writes the messages to the console.
        /// </summary>
        /// <param name="ToWrite">The message</param>
        /// <param name="Program">Name of the program this message should be displayed as coming from.</param>
        public static void WriteAll(Writable[] ToWrite, string Program)
        {
            ConsoleColor Old = Console.ForegroundColor;

            BasicWriteLine("______________________________________________________");
            BasicWriteLine(Time + "(" + Program + ")");

            foreach (Writable item in ToWrite)
            {
                WriteLine(item);
            }

            BasicWriteLine("______________________________________________________");
            Console.ForegroundColor = Old;
        }

        /// <summary>
        /// Same thing as the other WriteAll, except this assumes this is internal and doesn't display what program it is from.
        /// </summary>
        /// <param name="ToWrite"></param>
        public static void WriteAll(Writable[] ToWrite)
        {
            foreach (Writable item in ToWrite)
            {
                WriteLine(item);
            }
        }

        /// <summary>
        /// Writes the messages to the console.
        /// </summary>
        /// <param name="ToWrite">The message</param>
        /// <param name="Program">Name of the program this message should be displayed as coming from.</param>
        public static void WriteAll(List<Writable> ToWrite, string Program)
        {
            WriteAll(ToWrite.ToArray(), Program);
        }

        /// <summary>
        /// Same thing as the other WriteAll, except this assumes this is internal and doesn't display what program it is from.
        /// </summary>
        /// <param name="ToWrite"></param>
        public static void WriteAll(List<Writable> ToWrite)
        {
            WriteAll(ToWrite.ToArray());
        }

        /// <summary>
        /// Just another WriteAll implimentation.
        /// </summary>
        /// <param name="ToWrite"></param>
        /// <param name="Program"></param>
        public static void WriteAll(string[] ToWrite, string Program)
        {
            List<Writable> Write = new List<Writable>();

            foreach (string item in ToWrite)
            {
                Write.Add(new Writable(item));
            }

            WriteAll(Write, Program);
        }
    }
}
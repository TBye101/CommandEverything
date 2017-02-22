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
            ConsoleColor Old = Console.BackgroundColor;
            Console.BackgroundColor = ToWrite.Color;
            Console.WriteLine("______________________________________________________");
            Console.WriteLine(Time + "(" + Program + ")");
            Console.WriteLine(Time + " " + ToWrite.Message);
            Console.WriteLine("______________________________________________________");
            Console.BackgroundColor = Old;
        }

        /// <summary>
        /// Same thing as the other Writeline, except this assumes this is internal and doesn't display what program it is from.
        /// </summary>
        /// <param name="ToWrite"></param>
        public static void WriteLine(Writable ToWrite)
        {
            ConsoleColor Old = Console.BackgroundColor;
            Console.BackgroundColor = ToWrite.Color;
            Console.WriteLine(Time + " " + ToWrite.Message);
            Console.BackgroundColor = Old;
        }

        public static void WriteLine(string ToWrite)
        {
            Console.WriteLine(Time + " " + ToWrite);
        }

        public static void WriteLine(string ToWrite, string Program)
        {
            Console.WriteLine("______________________________________________________");
            Console.WriteLine(Time + "(" + Program + ")");
            Console.WriteLine(Time + " " + ToWrite);
            Console.WriteLine("______________________________________________________");
        }

        /// <summary>
        /// Writes the messages to the console.
        /// </summary>
        /// <param name="ToWrite">The message</param>
        /// <param name="Program">Name of the program this message should be displayed as coming from.</param>
        public static void WriteAll(Writable[] ToWrite, string Program)
        {
            ConsoleColor Old = Console.BackgroundColor;

            Console.WriteLine("______________________________________________________");
            Console.WriteLine(Time + "(" + Program + ")");

            foreach (Writable item in ToWrite)
            {
                WriteLine(item);
            }

            Console.WriteLine("______________________________________________________");
            Console.BackgroundColor = Old;
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
    }
}
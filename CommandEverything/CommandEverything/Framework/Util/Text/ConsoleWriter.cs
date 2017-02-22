using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CommandEverything.Framework.Util
{
    /// <summary>
    /// Knows how to do more advanced kinds of output to the console.
    /// </summary>
    public class ConsoleWriter
    {
        /// <summary>
        /// Writes the message to the console.
        /// </summary>
        /// <param name="ToWrite">The message</param>
        /// <param name="Program">Name of the program this message should be displayed as coming from.</param>
        public void WriteLine(Writable ToWrite, string Program)
        {

        }

        /// <summary>
        /// Writes the messages to the console.
        /// </summary>
        /// <param name="ToWrite">The message</param>
        /// <param name="Program">Name of the program this message should be displayed as coming from.</param>
        public void WriteAll(Writable[] ToWrite, string Program)
        {

        }

        /// <summary>
        /// Writes the messages to the console.
        /// </summary>
        /// <param name="ToWrite">The message</param>
        /// <param name="Program">Name of the program this message should be displayed as coming from.</param>
        public void WriteAll(List<Writable> ToWrite, string Program)
        {

        }
    }
}
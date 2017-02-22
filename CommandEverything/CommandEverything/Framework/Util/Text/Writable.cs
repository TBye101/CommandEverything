using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CommandEverything.Framework.Util.Text
{
    /// <summary>
    /// Used to store information that is used by the ConsoleWriter.
    /// Used to specify how the ConsoleWriter writes things.
    /// </summary>
    public class Writable
    {
        /// <summary>
        /// The color of the line.
        /// </summary>
        public ConsoleColor Color;

        /// <summary>
        /// The message of the line.
        /// </summary>
        public string Message;

        public Writable(ConsoleColor TheColor, string TheMessage)
        {
            this.Color = TheColor;
            this.Message = TheMessage;
        }
        public Writable(string TheMessage)
        {
            this.Message = TheMessage;
            this.Color = ConsoleColor.Gray;
        }
    }
}
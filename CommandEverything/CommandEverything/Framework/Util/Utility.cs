using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CommandEverything.Framework.Util
{
    /// <summary>
    /// A utility class.
    /// </summary>
    public static class Utility
    {
        /// <summary>
        /// Removes a word from a string.
        /// </summary>
        /// <param name="FullString"></param>
        /// <param name="WordToRemove"></param>
        /// <returns></returns>
        public static string RemoveWordFromString(string FullString, string WordToRemove)
        {
            int index = FullString.IndexOf(WordToRemove);
            int index2 = index + WordToRemove.Length;

            if (index != -1)
            {
                string Result = FullString.Remove(index, index2 - index);
                Result = Result.Trim();
                return Result;
            }

            return "Error";
        }

        /// <summary>
        /// Returns true if any of the words in DoesItContain are in DoIContain.
        /// </summary>
        /// <param name="DoIContain"></param>
        /// <param name="DoesItContain"></param>
        /// <returns></returns>
        public static bool DoesStringContain(string DoIContain, string[] DoesItContain)
        {
            foreach (string item in DoesItContain)
            {
                if (DoIContain.Contains(item))
                {
                    return true;
                }
            }

            return false;
        }

        /// <summary>
        /// Converts a string array to a string. Assuming that each line needs a line terminator.
        /// </summary>
        /// <param name="Array"></param>
        /// <returns></returns>
        public static string ConvertArray(string[] Array)
        {
            StringBuilder Ret = new StringBuilder();

            foreach (string item in Array)
            {
                Ret.Append(item);
                Ret.Append("\r\n");
            }

            return Ret.ToString();
        }

        /// <summary>
        /// Converts a byte array to binary.
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public static string ToBinary(byte[] data)
        {
            return string.Join(" ", data.Select(byt => Convert.ToString(byt, 2).PadLeft(8, '0')));
        }

        /// <summary>
        /// Creates a string from a collection of numbers.
        /// </summary>
        /// <param name="Numbers"></param>
        /// <returns></returns>
        public static string StringFrom(List<int> Numbers)
        {
            StringBuilder Ret = new StringBuilder();

            foreach (int item in Numbers)
            {
                Ret.Append(item.ToString());
            }

            return Ret.ToString();
        }
    }
}
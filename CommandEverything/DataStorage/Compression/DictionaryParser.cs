using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataStorage.Compression
{
    /// <summary>
    /// Creates a dictionary object out of the specified text file, assuming it is formatted correctly.
    /// </summary>
    public static class DictionaryParser
    {
        /// <summary>
        /// Creates a dictionary object out of the specified file.
        /// </summary>
        /// <param name="File"></param>
        /// <returns></returns>
        public static Dictionary<string, string> ParseFile(string File)
        {
            Dictionary<string, string> Ret = new Dictionary<string, string>();

            string[] Lines = File.Split("\r\n".ToCharArray(), StringSplitOptions.RemoveEmptyEntries);

            string[] Values = { "", "" };
            foreach (string item in Lines)
            {
                Values = item.Split(',');
            }

            Ret.Add(Values[0], Values[1]);

            return Ret;
        }
    }
}
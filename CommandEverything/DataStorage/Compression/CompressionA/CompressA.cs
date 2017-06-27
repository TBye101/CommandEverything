using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace DataStorage.Compression.CompressionA
{
    /// <summary>
    /// Used to compress and decompress strings.
    /// </summary>
    public static class CompressA
    {
        /// <summary>
        /// Stores the current compression dictionary.
        /// </summary>
        private static Dictionary<string, string> CompressionDictionary = DictionaryParser.ParseFile(Resources.CompressionDictionaryA.ToString());

        /// <summary>
        /// Compresses the given string and returns it.
        /// </summary>
        /// <param name="ToCompress"></param>
        /// <returns></returns>
        public static string Compress(string ToCompress)
        {
            string Binary = ToBinary(ConvertToByteArray(ToCompress, Encoding.ASCII));
            Binary = Binary.Replace(" ", "");

            int i = 0;
            int size = CompressionDictionary.Count;

            while (i != size)
            {
                string key = CompressionDictionary.ElementAt(i).Key.Trim();
                string value = CompressionDictionary.ElementAt(i).Value.Trim();

                Binary = Binary.Replace(key, value);

                i++;
            }

            return Binary;
        }

        /// <summary>
        /// Decompresses the given string.
        /// </summary>
        /// <param name="ToDecompress"></param>
        /// <returns></returns>
        public static string Decompress(string ToDecompress)
        {
            string Binary = Encoding.ASCII.GetBytes(ToDecompress).ToString();

            int i = 0;
            int size = CompressionDictionary.Count;

            while (i != size)
            {
                string key = CompressionDictionary.ElementAt(size - 1).Key;
                string value = CompressionDictionary.ElementAt(size - 1).Value;

                Binary = Binary.Replace(value, key);

                i++;
            }

            return Binary;
        }

        private static byte[] ConvertToByteArray(string str, Encoding encoding)
        {
            return encoding.GetBytes(str);
        }

        /// <summary>
        /// Converts a byte array to binary.
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        private static string ToBinary(byte[] data)
        {
            return string.Join(" ", data.Select(byt => Convert.ToString(byt, 2).PadLeft(8, '0')));
        }
    }
}
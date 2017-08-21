#pragma once

namespace performance
{

#pragma region Types

	namespace types
	{
		/// <summary>
		/// A string implementation.
		/// Unsigned __int8 size should be a positive number greater than 0.
		/// </summary>
		template <unsigned __int8 size>
		struct string8
		{
			char Characters[size];

			/// <summary>
			/// Gets the size of this string.
			/// </summary>
			/// <returns></returns>
			unsigned register __int8 Size()
			{
				return (sizeof(Characters) / sizeof(char));
			}
		};
	};

#pragma endregion


};
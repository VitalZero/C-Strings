#include <fstream>
#include <conio.h>

namespace vz 
{
	void printfixed(const char* buffer, int width)
	{
		int n = 0;
		for (; *buffer != 0; buffer++)
		{
			++n;
			_putch(*buffer);
		}
		for (; n < width; ++n)
		{
			_putch(' ');
		}
	}

	void print(const char* buffer)
	{
		for (; *buffer != 0; buffer++)
		{
			_putch(*buffer);
		}
	}

	void read(char* buffer, int maxSize)
	{
		const char* const pEnd = buffer + maxSize;
		for (char c = _getch(); c != 13 && (buffer + 1 < pEnd); c = _getch(), buffer++)
		{
			_putch(c);
			*buffer = c;
		}
		*buffer = 0;
	}

	int str2int(const char* buffer)
	{
		const char* pBuffer = buffer;
		for (; *pBuffer >= '0' && *pBuffer <= '9'; pBuffer++);
		pBuffer--;

		int value = 0;
		int factor = 1;
		for (; pBuffer >= buffer; pBuffer--)
		{
			value += (*pBuffer - '0') * factor;
			factor *= 10;
		}

		return value;
	}
	void strrev(char* buffer)
	{
		int size = 0;
		char* pEnd = buffer;

		for (; *pEnd != 0; ++pEnd);
		pEnd--;

		for (; buffer < pEnd; ++buffer, --pEnd)
		{
			const char temp = *buffer;
			*buffer = *pEnd;
			*pEnd = temp;
		}
	}

	void strcpy(const char* source, char* dest, int maxSize)
	{
		for (int i = 0; *source != 0 && (i + 1 < maxSize); ++source, ++dest, ++i)
		{
			*dest = *source;
		}
		*dest = 0;
	}

	void int2str(char* buffer, int maxSize, int value)
	{
		const char* pEnd = buffer + maxSize;
		char* pBeg = buffer;
		for (; (pBeg + 1 < pEnd) && value != 0; ++pBeg, value /= 10)
		{
			*pBeg = value % 10 + '0';
		}

		*pBeg = 0;
		strrev(buffer);
	}

	unsigned int fibonacci(int n)
	{
		if (n < 2)
			return n;

		return fibonacci(n - 1) + fibonacci(n - 2);
	}

	class Database
	{
	private:
		class Entry
		{
		public:
			Entry() = default;
			Entry(const char* name, int value)
				: value(value)
			{
				strcpy(name, this->name, nameBufferSize);
			}
			void Print() const
			{
				printfixed(name, nameBufferSize - 1);
				_putch('|');
				for (int i = 0; i < value; ++i)
				{
					_putch('=');
				}
				_putch('\n');
			}
			void Serialize(std::ofstream& out) const
			{
				out.write(name, sizeof(name));
				out.write(reinterpret_cast<const char*>(&value), sizeof(value));
			}
			void Deserialize(std::ifstream& in)
			{
				in.read(name, sizeof(name));
				in.read(reinterpret_cast<char*>(&value), sizeof(value));
			}
		private:
			static constexpr int nameBufferSize = 10;
			char name[nameBufferSize];
			int value;
		};
	public:
		Database() = default;
		void Load(const char* fileName)
		{
			std::ifstream in(fileName, std::ios::binary);
			in.read(reinterpret_cast<char*>(&curNumberEntries), sizeof(curNumberEntries));
			for (int i = 0; i < curNumberEntries; ++i)
			{
				entries[i].Deserialize(in);
			}
		}
		void Save(const char* fileName) const
		{	
			std::ofstream out(fileName, std::ios::binary);
			out.write(reinterpret_cast<const char*>(&curNumberEntries), sizeof(curNumberEntries));
			for (int i = 0; i < curNumberEntries; ++i)
			{
				entries[i].Serialize(out);
			}
		}
		void Print() const
		{
			for (int i = 0; i < curNumberEntries; ++i)
			{
				entries[i].Print();
			}
		}
		void Add(const char* name, int value)
		{
			if (curNumberEntries < maxNumberEntries)
			{
				entries[curNumberEntries++] = { name, value };
			}
		}
	private:
		static constexpr int maxNumberEntries = 16;
		Entry entries[maxNumberEntries];
		int curNumberEntries = 0;
	};
}

int main()
{
	vz::Database db;
	char buffer[256];
	char buffer2[256];
	bool quitting = false;

	do
	{
		vz::print("(l)oad (s)ave (a)dd (q)uit or (p)rint? ");
		char response = _getch();

		switch (response)
		{
		case 'l':
			vz::print("\nEnter filename: ");
			vz::read(buffer, sizeof(buffer));
			db.Load(buffer);
			_putch('\n');
			break;
		case 's':
			vz::print("\nEnter filename: ");
			vz::read(buffer, sizeof(buffer));
			db.Save(buffer);
			_putch('\n');
			break;
		case 'a':
			vz::print("\nEnter Name: ");
			vz::read(buffer, sizeof(buffer));
			vz::print("\nEnter Value: ");
			vz::read(buffer2, sizeof(buffer2));
			db.Add(buffer, vz::str2int(buffer2));
			_putch('\n');
			break;
		case 'p':
			vz::print("\n    Chart");
			vz::print("\n    =====\n\n");
			db.Print();
			_putch('\n');
			break;
		case 'q':
			quitting = true;
			break;
		}
	} while (!quitting);

	return 0;
}
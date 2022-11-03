float unit_conversion (char* unit, int count, float size)
{
		while (((int)size/1024) != 0)
		{
			size = size/1024;
			count++;
		}
		if (count == 1)
		{
			*unit = 'M';
		}
		else if (count == 2)
		{
			*unit = 'G';
		}

		return size;
}

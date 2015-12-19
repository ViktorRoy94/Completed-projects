int comp_int(const void*pa, const void*pb) {
	int a = *((const int*)pa);
	int b = *((const int*)pb);
	if (a<b)
		return -1;
	else
		return (a == b) ? (0) : (1);
}
#include <stdio.h>

int main(void)
{
	FILE *input, *output;
	unsigned long i;
	unsigned int tmp;

	input = fopen("vga.pcf", "r");
	output = fopen("bdatapcf.h", "w");

	fprintf(output, "char bdata[]={");
	for (i = 0; i < 2820; i++) {
		tmp = getc(input);
		fprintf(output, "%d,", tmp);
	}
	fprintf(output, "};");

	fclose(input);
	fclose(output);

	input = fopen("vga.pcf", "r");
	output = fopen("edatapcf.h", "w");

	fseek(input, 19073, 0);

	fprintf(output, "char edata[]={");
	for (i = 0; i < 5559; i++) {
		tmp = getc(input);
		fprintf(output, "%d,", tmp);
	}
	fprintf(output, "};");
	fclose(output);
	return 0;
}

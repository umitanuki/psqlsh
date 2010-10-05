/* --------------------------------------------------
 *  psqlsh.c
 *    A bridge module to handle shebang with psql
 * ------------------------------------------------*/

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

void
create_temp(const char *filename, char *new_filename)
{
	FILE   *fin, *fout;
	char	buf[BUFSIZ];
	size_t  len;

	fin = fopen(filename, "r");
	if (!fin)
	{
		fprintf(stderr, "Could not Open %s", filename);
		return;
	}
	strcpy(new_filename, filename);
	strcat(new_filename, ".psqlsh");
	fout = fopen(new_filename, "w");
	if (!fout)
	{
		fprintf(stderr, "Could not Open %s", new_filename);
		return;
	}
	/* read out the first line shebang */
	fgets(buf, BUFSIZ, fin);
	if (!(buf[0] == '#' && buf[1] == '!'))
	{
		fputs(buf, fout);
	}
	while((len = fread(buf, sizeof(char), BUFSIZ, fin)) > 0)
	{
		fwrite(buf, sizeof(char), len, fout);
	}

	fclose(fin);
	fclose(fout);
}

int
main(int argc, char **argv)
{
	char   *new_argv[256];
	char   *filename;
	char	new_filename[256];
	int		i;

	memset(new_argv, 0, sizeof(char *) * 256);
	new_argv[0] = "psql";
	if (argc > 1)
	{
		for(i = 1; i < argc - 1; i++)
		{
			new_argv[i] = argv[i];
		}
		filename = argv[i];
		create_temp(filename, new_filename);

		new_argv[i++] = "-q";
		new_argv[i++] = "-f";
		new_argv[i++] = new_filename;
	}

	if(execvp("psql", new_argv) == -1)
	{
		fprintf(stderr, "exec %s failed:%s\n", "psql", strerror(errno));
	}

	return errno;
}


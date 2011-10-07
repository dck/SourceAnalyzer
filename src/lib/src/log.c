void _log (int level, const char* file, uint32 line, const char* func, const char* msg) {
#ifdef INTERNAL_PARAM_CHECKS
#endif

	if (errorlevel == level)
		fprintf(stderr, "error ");
	if (NULL != file)
		fprintf(stderr, "%s",  file);
	if (0 != line)
		fprintf(stderr, " %d", line);
	if (NULL != func)
		fprintf(stderr, " %s", func);
	if (NULL != msg)
		fprintf(stderr, " %s", msg);
	fprintf(stderr, "\n");
}


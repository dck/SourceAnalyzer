#ifndef APICALL_H
#define APICALL_H

/* should be function specific, in other words __func__ name must
 be a part of lable */
#define CLEANUP cleanup##__func__

#ifdef TRACE
	#define TRACE_BEFORE_CALL (call)       fprintf (stderr, "---> %s\n", #call)
	#define TRACE_AFTER_CALL  (call, sts)  fprintf (stderr, "%d <- %s\n", sts, #call)
#else
	#define TRACE_BEFORE_CALL (call)
	#define TRACE_AFTER_CALL  (call, sts)
#endif

#define GOTO_CLEANUP_IF (cond, saret, err)                           \
	if (cond) {                                                  \
	#ifdef ERROR_LOG                                             \
		fprintf (stderr, "error: %s: %s: %l: code: %d\n",    \
			 __FILE__, __func__, __LINE__, (err));       \
	#endif                                                       \
		saret = (err);                                       \
		goto CLEANUP;                                        \
	}

#define CALL_API (sts, call)    { TRACE_BEFORE_CALL (call);             \
				  sts = call;                           \
				  TRACE_AFTER_CALL  (call, sts);        \
				  GOTO_CLEANUP_IF (0 != sts, sts, sts); }

#define CALL_BD_API (sts, call) { int __tmp = 0;                                 \
				  TRACE_BEFORE_CALL (call);                      \
				  __tmp = call;                                  \
				  TRACE_BEFORE_CALL (call, __tmp);               \
				  GOTO_CLEANUP_IF (0 != __tmp && DB_NOTFOUND != __tmp, sts, SA3_DB_ERR); }

#endif // APICALL_H

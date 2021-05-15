/*
	Signal number definitions
	EZ -- Jul 14, 1985
*/


/*
	Cromix signals
*/

#define sigabort        1       /* Control-C key                        */
#define siguser         2       /* User specifiable key                 */
#define sigkill         3       /* Kill signal                          */
#define sigterm         4       /* Terminate                            */
#define sigalarm        5       /* Alarm                                */
#define sigpipe         6       /* Broken pipe signal                   */
#define sighangup       7       /* Modem hang up                        */
#define sigtrace        8       /* Trace (or breakpoint)                */

#define SIGABORT        (1 << sigabort - 1)
#define SIGUSER         (1 << siguser  - 1)
#define SIGKILL         (1 << sigkill  - 1)
#define SIGTERM         (1 << sigterm  - 1)
#define SIGALARM        (1 << sigalarm - 1)
#define SIGPIPE         (1 << sigpipe  - 1)
#define SIGHANGUP       (1 << sighangup- 1)
#define SIGTRACE        (1 << sigtrace - 1)

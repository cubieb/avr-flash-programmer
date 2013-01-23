#ifndef	_CMD_OPTIONS_H_
#define _CMD_OPTIONS_H_


struct cmd_argv{
	unsigned char argc;
	char *argv;
};

/*
 * functions list
 */
int show_help(void *cmd_argv);
int echo_string(void *cmd_argv);
int file_upload(void *argv);
int file_download(void *cmd_argv);
int flash_read_status(void *cmd_argv);
int flash_dump(void *cmd_argv);
int flash_serase(void *cmd_argv);
int flash_berase(void *cmd_argv);
int flash_cerase(void *cmd_argv);


#endif	/* _CMD_OPTIONS_H_ */

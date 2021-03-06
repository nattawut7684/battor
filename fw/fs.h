#ifndef FS_H
#define FS_H

#define FS_SUPERBLOCK_IDX 0
#define FS_VERSION 3

#define FS_FILE_SKIP_LEN 250

#define FS_LAST_FILE_BLOCKS 50000

extern uint32_t g_fs_file_seq;

typedef struct fs_superblock_
{
	uint8_t magic[8];
	uint8_t ver;
	uint32_t fmt_iter;
	uint8_t portable;
} fs_superblock;

typedef struct fs_file_startblock_
{
	uint32_t fmt_iter;
	uint32_t seq;
	uint32_t byte_len;
	uint32_t next_skip_file_startblock_idx;
	uint32_t rtc_start_time_s;
	uint32_t rtc_start_time_ms;
} fs_file_startblock;

typedef enum FS_ERROR_enum
{
	FS_ERROR_NONE = 0,
	FS_ERROR_EOF = -1,
	FS_ERROR_NO_EXISTING_FILE = -2,
	FS_ERROR_FILE_TOO_LONG = -3,
	FS_ERROR_FILE_NOT_OPEN = -4,
	FS_ERROR_SD_READ = -5,
	FS_ERROR_SD_WRITE = -6,
	FS_ERROR_WRITE_IN_PROGRESS = -7,
	FS_ERROR_CANNOT_READ_NEW_FILE = -8,
	FS_ERROR_SD_MULTI_WRITE_END = -9,
	FS_ERROR_SEEK_PAST_END = -10,
} FS_ERROR_t;

int fs_info(fs_superblock* sb);
int fs_format(uint8_t portable);
int fs_open(uint8_t create_file, uint32_t file_seq_to_open);
int fs_seek(uint32_t pos);
int fs_rtc_get(uint32_t* s, uint32_t* ms);
int fs_rtc_set();
int32_t fs_size_get();
int fs_close();
int fs_write(void* buf, uint16_t len);
int fs_read(void* buf, uint16_t len);
void fs_update();
int fs_busy();
int fs_self_test();

#endif

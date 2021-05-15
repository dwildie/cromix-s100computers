typedef struct {
  unsigned int cylinders;
  unsigned int track_sectors;
  unsigned int surfaces;
  unsigned int sector_bytes;
  unsigned int start_cylinder;
  unsigned int start_part_table;
  unsigned int start_alt_table;
  unsigned int alt_track_cyl;
  unsigned int alt_tracks;
  unsigned char identifier[3];
} disk_geometry;

typedef struct {
  int status;
  int type;
  unsigned int start;
  unsigned int sectors;
} partition;

typedef struct {
  partition part;
  disk_geometry geom;
  dcb dcb;
} disk_info;

typedef struct {
  unsigned char status;
  disk_info disks[MBR_PARTITIONS];
} drive_info;

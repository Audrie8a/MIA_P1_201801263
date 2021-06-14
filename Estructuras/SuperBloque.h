#ifndef SuperBloque_H
#define SuperBloque_H

struct TablaInodos
{
    int uid = -1; //Uid del propietario del archivo
    int gid;      //gid del grupo al que pertenece el archivo
    int size;     //Tamaño del archivo en bytes
    time_t atime; //Fecha de lectura de inodo sin modificacion
    time_t ctime; //Fecha en la que se creo el inodo
    time_t mtime; //Ultima fecha de modificcacion del inodo
    int block[15] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    char type; //Archivo -> 1 o carpeta -> 0
    int perm = -1;
};

struct Content{
    char name[12];
    int inodo=-1;
};

struct BloqueApuntadores{
    int b_pointers[16]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
};

struct BloqueCarpeta{
    Content content[4];
};

struct BloqueArchivo
{
    char content[64];
};

struct SuperBloque
{
    int filesystem_type;
    int inodes_count;
    int blocks_count;
    int free_blocks_count;
    int free_inodes_count;
    time_t  Mtime;  //Ultima fecha montado
    time_t  Umtime; //Ultima fecha desmontado
    int mnt_count; //Cuantas veces se ha montado el sistema
    int magic = 0xEF53;
    int inode_size = sizeof(TablaInodos);
    int block_size = sizeof(BloqueArchivo);
    int fist_ino = 0;   //Primer inodo libre
    int first_blo = 0;  //Primer bloque libre
    int bm_inode_start; //Inicio del bitmap de inodos
    int bm_block_start; //Inicio del bitmap de bloques
    int inode_start;    //Inicio de tabla de inodos
    int block_start;    //Inicio de tabla de bloques
};

struct Journaling
{    
    char tipo = '7';
    char path[80] = "";
    char operacion[10] = "vacio";
    char contenido[64] = ""; // Validar cuando espacio necesitan.
    time_t log_fecha;
    int size = 0;
    int InodoAfectado = -1;
};
#endif
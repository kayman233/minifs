cmake_minimum_required(VERSION 3.10)

# set the project name
project(Minifs)

# add the executable
set(SOURCE_FILES_INIT ./init.c ./src/io/fsio.c ./src/data_structures/superblock.c ./src/data_structures/sector.c ./src/data_structures/inode.c)
set(SOURCE_FILES_RUN ./main.c ./src/io/fsio.c ./src/data_structures/superblock.c ./src/data_structures/sector.c ./src/data_structures/inode.c ./src/commands/commands.c)

add_executable(init ${SOURCE_FILES_INIT})
add_executable(run ${SOURCE_FILES_RUN})
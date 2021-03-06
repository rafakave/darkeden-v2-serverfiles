#--------------------------------------------------------------------------------
#
# Filename    : Makefile
# Written By  : Reiot
#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# Directories
#--------------------------------------------------------------------------------
VSHOME  = ../../..
STLDIR  = /usr/include/STL
BINDIR  = $(VSHOME)/bin
LIBDIR  = $(VSHOME)/lib
SRCDIR  = $(VSHOME)/src
COREDIR = $(SRCDIR)/Core
SERVDIR = $(SRCDIR)/server
TOSDIR  = $(SERVDIR)/theoneserver
GSDIR   = $(SERVDIR)/gameserver
DBDIR	= $(SERVDIR)/database
LSDIR   = $(SERVDIR)/loginserver


#--------------------------------------------------------------------------------
# General Rules
#--------------------------------------------------------------------------------
.SUFFIXES : .cpp .tos.o .gs.o

CC       = g++
DEFS     = -D__LINUX__ -D_REENTRANT
INCLUDES = -I$(STLDIR) -I$(COREDIR) -I$(GSDIR) -I$(TOSDIR) -I$(DBDIR) -I$(LSDIR) -I$(SERVDIR)
#INCLUDES = -I$(STLDIR) -I$(COREDIR) -I$(SERVDIR) -I$(TOSDIR)
CCFLAGS  = -Wall -c -g 
#CCFLAGS  = -Wall -c -g 

TARGET1 = $(LIBDIR)/libTheOneServerPackets.a
TARGET2 = $(LIBDIR)/libGameServerPackets.a

OBJS1	= \
		GTOAcknowledgement.tos.o GTOAcknowledgementHandler.tos.o

OBJS2	= \
		GTOAcknowledgement.gs.o GTOAcknowledgementHandler.gs.o

SRCS	= $(OBJS1:.tos.o=.cpp)

.cpp.tos.o : 
	$(CC) $(CCFLAGS) $(DEFS) -D__THE_ONE_SERVER__ $(INCLUDES) -I$(TOSDIR) -I$(SERVDIR) -I$(DBDIR) -o $*.tos.o $<

.cpp.gs.o : 
	$(CC) $(CCFLAGS) $(DEFS) -D__GAME_SERVER__ $(INCLUDES) -I$(GSDIR) -I$(SERVDIR) -I$(DBDIR) -o $*.gs.o $<



#--------------------------------------------------------------------------------
# Actions
#--------------------------------------------------------------------------------

all : $(TARGET1) $(TARGET2)
#all : $(TARGET1)

alltheoneserver : $(TARGET1)

allgameserver : $(TARGET2)

$(TARGET1) : $(OBJS1)
	ar rc $@ $(OBJS1)

$(TARGET2) : $(OBJS2)
	ar r $@ $(OBJS2)

clean :
	rm -rf *.o

cleantheoneserver :
	rm -rf *.tos.o

cleangameserver :
	rm -rf *.gs.o

cleanall :
	rm -rf *.o $(TARGET1) $(TARGET2)

depall :
	gccmakedep  $(SRCS) $(INCLUDES) $(DEFS)

dep :
	gccmakedep  $(SRCS) $(INCLUDES) $(DEFS)

# DO NOT DELETE
GTOAcknowledgement.o: GTOAcknowledgement.cpp GTOAcknowledgement.h \
 /home/darkeden/vs/src/Core/DatagramPacket.h \
 /home/darkeden/vs/src/Core/Packet.h \
 /home/darkeden/vs/src/Core/Types.h \
 /home/darkeden/vs/src/Core/types/SystemTypes.h \
 /usr/include/i386-linux-gnu/sys/types.h /usr/include/features.h \
 /usr/include/i386-linux-gnu/sys/cdefs.h /usr/include/i386-linux-gnu/gnu/stubs.h \
 /usr/include/i386-linux-gnu/bits/types.h \
 /usr/lib/gcc/i686-linux-gnu/7/include/stddef.h \
 /usr/include/i386-linux-gnu/bits/pthreadtypes.h /usr/include/i386-linux-gnu/bits/sched.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/i386-linux-gnu/bits/endian.h \
 /usr/include/i386-linux-gnu/sys/select.h /usr/include/i386-linux-gnu/bits/select.h \
 /home/darkeden/src/libs/sigset.h /usr/include/i386-linux-gnu/bits/time.h \
 /usr/include/i386-linux-gnu/sys/sysmacros.h /usr/include/c++/7/iostream \
  /usr/include/c++/7/streambuf \
 /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
 /usr/include/i386-linux-gnu/bits/wchar.h /usr/include/gconv.h \
 /usr/lib/gcc/i686-linux-gnu/7/include/stdarg.h \
 /usr/include/c++/7/fstream  \
 /usr/include/c++/7/string /home/darkeden/src/libs/stl_config.h \
 /home/darkeden/src/libs/stl_string_fwd.h /home/darkeden/src/libs/stl_alloc.h \
 /usr/include/c++/7/new \
 /usr/include/c++/7/exception \
 /usr/include/stdlib.h /usr/include/alloca.h /usr/include/string.h \
 /usr/include/assert.h /home/darkeden/src/libs/stl_threads.h \
 /usr/include/pthread.h /usr/include/sched.h /usr/include/signal.h \
 /usr/include/i386-linux-gnu/bits/initspin.h /usr/lib/i386-linux-gnu/perl/5.26.1/bits/sigthread.ph \
 /usr/include/ctype.h /usr/include/c++/7/functional \
 /usr/include/c++/7/bits/stl_function.h /usr/include/c++/7/stdexcept \
 /home/darkeden/src/libs/stl_exception.h /home/darkeden/src/libs/stl_iterator_base.h \
 /usr/include/c++/7/memory /home/darkeden/src/libs/stl_algobase.h \
 /home/darkeden/src/libs/stl_relops.h /home/darkeden/src/libs/stl_pair.h \
 /usr/include/c++/7/ext/type_traits.h \
 /usr/lib/gcc/i686-linux-gnu/7/include-fixed/limits.h \
 /usr/lib/gcc/i686-linux-gnu/7/include-fixed/syslimits.h \
 /usr/include/limits.h /usr/include/i386-linux-gnu/bits/posix1_lim.h \
 /usr/include/i386-linux-gnu/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/i386-linux-gnu/bits/posix2_lim.h \
 /home/darkeden/src/libs/new.h \
 /home/darkeden/src/libs/stl_iterator.h /home/darkeden/src/libs/stl_construct.h \
 /home/darkeden/src/libs/stl_tempbuf.h /home/darkeden/src/libs/stl_uninitialized.h \
 /home/darkeden/src/libs/stl_raw_storage_iter.h /usr/include/c++/7/algorithm \
 /home/darkeden/src/libs/stl_algo.h /home/darkeden/src/libs/stl_heap.h \
 /usr/include/c++/7/bits/char_traits.h /usr/include/stdio.h \
 /home/darkeden/src/libs/stl_hash_fun.h \
 /home/darkeden/vs/src/Core/types/PlayerTypes.h \
 /home/darkeden/vs/src/Core/types/ObjectTypes.h \
 /home/darkeden/vs/src/Core/types/CreatureTypes.h \
 /home/darkeden/vs/src/Core/types/ItemTypes.h \
 /home/darkeden/vs/src/Core/types/ZoneTypes.h \
 /home/darkeden/vs/src/Core/types/QuestTypes.h \
 /home/darkeden/vs/src/Core/types/ShopTypes.h \
 /home/darkeden/vs/src/Core/types/GuildTypes.h \
 /home/darkeden/vs/src/Core/types/WarTypes.h \
 /home/darkeden/vs/src/Core/types/ServerType.h \
 /home/darkeden/vs/src/Core/Utility.h \
 /home/darkeden/vs/src/Core/Exception.h \
 /home/darkeden/vs/src/Core/StringStream.h /usr/include/c++/7/list \
 /home/darkeden/src/libs/stl_list.h \
 /home/darkeden/vs/src/Core/SocketInputStream.h \
 /home/darkeden/vs/src/Core/Socket.h \
 /home/darkeden/vs/src/Core/SocketImpl.h \
 /home/darkeden/vs/src/Core/SocketAPI.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/i386-linux-gnu/bits/wordsize.h \
 /usr/include/i386-linux-gnu/bits/socket.h /usr/include/i386-linux-gnu/bits/sockaddr.h \
 /usr/include/asm/socket.h /usr/include/asm/sockios.h \
 /usr/include/i386-linux-gnu/bits/in.h /usr/include/i386-linux-gnu/bits/byteswap.h \
 /usr/include/sys/socket.h /usr/include/sys/uio.h \
 /usr/include/bits/uio.h \
 /home/darkeden/vs/src/Core/SocketOutputStream.h \
 /home/darkeden/vs/src/Core/Datagram.h /usr/include/arpa/inet.h \
 /home/darkeden/vs/src/Core/PacketFactory.h
GTOAcknowledgementHandler.o: GTOAcknowledgementHandler.cpp \
 GTOAcknowledgement.h /home/darkeden/vs/src/Core/DatagramPacket.h \
 /home/darkeden/vs/src/Core/Packet.h \
 /home/darkeden/vs/src/Core/Types.h \
 /home/darkeden/vs/src/Core/types/SystemTypes.h \
 /usr/include/i386-linux-gnu/sys/types.h /usr/include/features.h \
 /usr/include/i386-linux-gnu/sys/cdefs.h /usr/include/i386-linux-gnu/gnu/stubs.h \
 /usr/include/i386-linux-gnu/bits/types.h \
 /usr/lib/gcc/i686-linux-gnu/7/include/stddef.h \
 /usr/include/i386-linux-gnu/bits/pthreadtypes.h /usr/include/i386-linux-gnu/bits/sched.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/i386-linux-gnu/bits/endian.h \
 /usr/include/i386-linux-gnu/sys/select.h /usr/include/i386-linux-gnu/bits/select.h \
 /home/darkeden/src/libs/sigset.h /usr/include/i386-linux-gnu/bits/time.h \
 /usr/include/i386-linux-gnu/sys/sysmacros.h /usr/include/c++/7/iostream \
  /usr/include/c++/7/streambuf \
 /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
 /usr/include/i386-linux-gnu/bits/wchar.h /usr/include/gconv.h \
 /usr/lib/gcc/i686-linux-gnu/7/include/stdarg.h \
 /usr/include/c++/7/fstream  \
 /usr/include/c++/7/string /home/darkeden/src/libs/stl_config.h \
 /home/darkeden/src/libs/stl_string_fwd.h /home/darkeden/src/libs/stl_alloc.h \
 /usr/include/c++/7/new \
 /usr/include/c++/7/exception \
 /usr/include/stdlib.h /usr/include/alloca.h /usr/include/string.h \
 /usr/include/assert.h /home/darkeden/src/libs/stl_threads.h \
 /usr/include/pthread.h /usr/include/sched.h /usr/include/signal.h \
 /usr/include/i386-linux-gnu/bits/initspin.h /usr/lib/i386-linux-gnu/perl/5.26.1/bits/sigthread.ph \
 /usr/include/ctype.h /usr/include/c++/7/functional \
 /usr/include/c++/7/bits/stl_function.h /usr/include/c++/7/stdexcept \
 /home/darkeden/src/libs/stl_exception.h /home/darkeden/src/libs/stl_iterator_base.h \
 /usr/include/c++/7/memory /home/darkeden/src/libs/stl_algobase.h \
 /home/darkeden/src/libs/stl_relops.h /home/darkeden/src/libs/stl_pair.h \
 /usr/include/c++/7/ext/type_traits.h \
 /usr/lib/gcc/i686-linux-gnu/7/include-fixed/limits.h \
 /usr/lib/gcc/i686-linux-gnu/7/include-fixed/syslimits.h \
 /usr/include/limits.h /usr/include/i386-linux-gnu/bits/posix1_lim.h \
 /usr/include/i386-linux-gnu/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/i386-linux-gnu/bits/posix2_lim.h \
 /home/darkeden/src/libs/new.h \
 /home/darkeden/src/libs/stl_iterator.h /home/darkeden/src/libs/stl_construct.h \
 /home/darkeden/src/libs/stl_tempbuf.h /home/darkeden/src/libs/stl_uninitialized.h \
 /home/darkeden/src/libs/stl_raw_storage_iter.h /usr/include/c++/7/algorithm \
 /home/darkeden/src/libs/stl_algo.h /home/darkeden/src/libs/stl_heap.h \
 /usr/include/c++/7/bits/char_traits.h /usr/include/stdio.h \
 /home/darkeden/src/libs/stl_hash_fun.h \
 /home/darkeden/vs/src/Core/types/PlayerTypes.h \
 /home/darkeden/vs/src/Core/types/ObjectTypes.h \
 /home/darkeden/vs/src/Core/types/CreatureTypes.h \
 /home/darkeden/vs/src/Core/types/ItemTypes.h \
 /home/darkeden/vs/src/Core/types/ZoneTypes.h \
 /home/darkeden/vs/src/Core/types/QuestTypes.h \
 /home/darkeden/vs/src/Core/types/ShopTypes.h \
 /home/darkeden/vs/src/Core/types/GuildTypes.h \
 /home/darkeden/vs/src/Core/types/WarTypes.h \
 /home/darkeden/vs/src/Core/types/ServerType.h \
 /home/darkeden/vs/src/Core/Utility.h \
 /home/darkeden/vs/src/Core/Exception.h \
 /home/darkeden/vs/src/Core/StringStream.h /usr/include/c++/7/list \
 /home/darkeden/src/libs/stl_list.h \
 /home/darkeden/vs/src/Core/SocketInputStream.h \
 /home/darkeden/vs/src/Core/Socket.h \
 /home/darkeden/vs/src/Core/SocketImpl.h \
 /home/darkeden/vs/src/Core/SocketAPI.h /usr/include/netinet/in.h \
 /usr/include/stdint.h /usr/include/i386-linux-gnu/bits/wordsize.h \
 /usr/include/i386-linux-gnu/bits/socket.h /usr/include/i386-linux-gnu/bits/sockaddr.h \
 /usr/include/asm/socket.h /usr/include/asm/sockios.h \
 /usr/include/i386-linux-gnu/bits/in.h /usr/include/i386-linux-gnu/bits/byteswap.h \
 /usr/include/sys/socket.h /usr/include/sys/uio.h \
 /usr/include/bits/uio.h \
 /home/darkeden/vs/src/Core/SocketOutputStream.h \
 /home/darkeden/vs/src/Core/Datagram.h /usr/include/arpa/inet.h \
 /home/darkeden/vs/src/Core/PacketFactory.h \
 /home/darkeden/vs/src/Core/Properties.h /usr/include/c++/7/map \
 /home/darkeden/src/libs/stl_tree.h /home/darkeden/src/libs/stl_map.h \
 /home/darkeden/src/libs/stl_multimap.h \
 /home/darkeden/vs/src/server/theoneserver/DB.h \
 /home/darkeden/vs/src/server/theoneserver/DatabaseManager.h \
 /usr/include/c++/7/backward/hash_map /home/darkeden/src/libs/stl_hashtable.h \
 /home/darkeden/src/libs/stl_vector.h /home/darkeden/src/libs/stl_hash_map.h \
 /home/darkeden/vs/src/server/theoneserver/Connection.h \
 /usr/local/include/mysql/mysql.h /usr/local/include/mysql/mysql_com.h \
 /usr/local/include/mysql/mysql_version.h \
 /home/darkeden/vs/src/server/Mutex.h \
 /home/darkeden/vs/src/server/theoneserver/Statement.h \
 /home/darkeden/vs/src/server/theoneserver/Result.h \
 /usr/include/i386-linux-gnu/sys/time.h \
 /home/darkeden/vs/src/Core/Gpackets/GGCommand.h \
 /home/darkeden/vs/src/server/theoneserver/GameServerManager.h \
 /home/darkeden/vs/src/server/Thread.h \
 /home/darkeden/vs/src/server/pthreadAPI.h \
 /home/darkeden/vs/src/Core/DatagramSocket.h \
 /home/darkeden/vs/src/server/Timeval.h /usr/include/unistd.h \
 /usr/include/i386-linux-gnu/bits/posix_opt.h /usr/include/i386-linux-gnu/bits/confname.h \
 /usr/include/getopt.h

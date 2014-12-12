package = 'xitari'
version = '0-0'

source = {
   url = ''
}

description = {
  summary = "Xitari"
}

dependencies = { 'torch >= 7.0' }
build = {
     type = "command",
     build_command = [[
         cmake . -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="$(LUA_BINDIR)/.." -DCMAKE_INSTALL_PREFIX="$(PREFIX)/../../../../../";
         $(MAKE) -j 4
            ]],
               install_command = "$(MAKE) install"
     }

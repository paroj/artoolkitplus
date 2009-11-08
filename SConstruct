vrs = Variables()
vrs.Add("prefix", "installation directory", "/usr")

env = Environment(variables=vrs)
env["CPPFLAGS"] = "-Wall"
env["CPPPATH"] = "include/"

sources = ["src/core/byteSwap.cpp"] \
          +Glob("src/*.cpp") \
          +Glob("src/extra/*.cpp") \
          +Glob("src/librpp/*.cpp")

#env["CPPDEFINES"] = ["ARTOOLKITPLUS_DLL"]
arlib = env.SharedLibrary("ARToolkitPlus", sources)

env.Install(env.get("prefix")+"/lib", arlib)
env.Install(env.get("prefix")+"/include", Glob("include/*"))

env.Alias("install", env.get("prefix"))

# uncomment to enable building of tools/ examples
# SConscript("bin/SConscript")

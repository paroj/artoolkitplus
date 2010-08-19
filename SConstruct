vrs = Variables()
vrs.Add("prefix", "installation directory", "/usr")

env = Environment(variables=vrs)
env["CPPFLAGS"] = "-Wall -g -O3"
env["CPPPATH"] = "include/"

sources = Glob("src/*.cpp") \
		  +Glob("src/core/*.cpp") \
          +Glob("src/extra/*.cpp") \
          +Glob("src/librpp/*.cpp")

arlib = env.SharedLibrary("ARToolKitPlus", sources)

env.Install(env.get("prefix")+"/lib", arlib)
env.Install(env.get("prefix")+"/include", Glob("include/*"))

env.Alias("install", env.get("prefix"))

# uncomment to enable building of tools/ and examples
SConscript("bin/SConscript")

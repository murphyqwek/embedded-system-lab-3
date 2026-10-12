"""Run host regression tests with GCC, Clang or TinyCC; no board required."""
import os
from pathlib import Path
import shutil
import subprocess
import sys

tests = Path(__file__).resolve().parent
project = tests.parent
compiler = os.environ.get("CC")
if not compiler:
    compiler = next((p for name in ("gcc", "clang", "tcc") if (p := shutil.which(name))), None)
if not compiler:
    bundled = Path(sys.prefix) / "Lib/site-packages/triton/runtime/tcc/tcc.exe"
    if bundled.exists():
        compiler = str(bundled)
if not compiler:
    raise SystemExit("Set CC to a host C compiler (GCC, Clang or TinyCC).")

sources = [tests / "test_snake.c"]
sources += [project / "Core/Src/snake" / (name + ".c")
            for name in ("snake", "snake_alloc", "field", "util", "apple_generator", "render")]
build = tests / ".build"
build.mkdir(exist_ok=True)
executable = build / ("test_snake.exe" if os.name == "nt" else "test_snake")
subprocess.run([compiler, "-std=c11", "-Wall", "-Werror",
                "-I" + str(tests / "stubs"), "-I" + str(project / "Core/Inc"),
                *map(str, sources), "-o", str(executable)], check=True)
subprocess.run([str(executable)], check=True)
oled_executable = build / ("test_oled.exe" if os.name == "nt" else "test_oled")
subprocess.run([compiler, "-std=c11", "-Wall", "-Werror",
                "-I" + str(tests / "stubs"), "-I" + str(project / "Core/Inc"),
                str(tests / "test_oled.c"), str(project / "Core/Src/fonts.c"),
                "-o", str(oled_executable)], check=True)
subprocess.run([str(oled_executable)], check=True)

param(
    [switch]$CompileOnly,
    [switch]$Test
)

$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$build = Join-Path $root "build"
$dosbox = "C:\Users\truni\AppData\Local\Programs\DOSBoxStaging\dosbox-staging-v0.83.0\dosbox.exe"
$turboRoot = "C:\Users\truni\AppData\Local\Programs\TurboCPP"
$source = Join-Path $root "option_16.cpp"
$dosSource = Join-Path $build "OPTION16.CPP"
$executable = Join-Path $build "OPTION16.EXE"

New-Item -ItemType Directory -Force -Path $build | Out-Null
$code = [IO.File]::ReadAllText($source, [Text.Encoding]::UTF8)
if ($Test) {
    $code = $code.Replace("#define T 600", "#define T 1")
    $code = $code.Replace("for (i = 0;; i++)", "for (i = 0; i < 4; i++)")
    $code = $code.Replace("getch();", "(void)0;")
    [IO.File]::WriteAllText((Join-Path $build "INPUT.TXT"), "5`r`n1.5`r`n", [Text.Encoding]::ASCII)
}
[IO.File]::WriteAllText($dosSource, $code, [Text.Encoding]::GetEncoding(866))
[IO.File]::WriteAllText(
    (Join-Path $build "BUILD.BAT"),
    "@ECHO OFF`r`nSET PATH=C:\TC\BIN`r`nC:\TC\BIN\TCC.EXE -IC:\TC\INCLUDE -LC:\TC\LIB -nD:\ D:\OPTION16.CPP`r`nEXIT`r`n",
    [Text.Encoding]::ASCII
)
Remove-Item -Force -ErrorAction SilentlyContinue $executable

$mountC = "mount C `"$turboRoot`""
$mountD = "mount D `"$build`""
& $dosbox --noprimaryconf --nolocalconf --exit -c $mountC -c $mountD -c "D:" -c "D:\BUILD.BAT" -c "exit"
if (-not (Test-Path $executable)) {
    throw "Turbo C++ did not create OPTION16.EXE. Check the compiler output."
}
Write-Host "Build succeeded: $executable"

if ($CompileOnly) {
    return
}

if ($Test) {
    & $dosbox --noprimaryconf --nolocalconf --exit -c $mountC -c $mountD -c "D:" -c "D:\OPTION16.EXE < D:\INPUT.TXT" -c "exit"
} else {
    & $dosbox --noprimaryconf --nolocalconf --exit -c $mountC -c $mountD -c "D:" -c "D:\OPTION16.EXE" -c "exit"
}

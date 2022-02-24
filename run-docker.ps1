Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
$build=0
for( $i = 0; $i -lt $args.count; $i++) {
	if ($args[ $i ] -eq "/b") { $build=1 }
}

if ( $build -eq 1 ) {
	echo "Need to build docker image, building..."
	powershell.exe -File build-docker.ps1 -ExecutionPolicy Bypass
}

$env:DISPLAY = (
    Get-NetIPConfiguration |
    Where-Object {
        $_.IPv4DefaultGateway -ne $null -and
        $_.NetAdapter.Status -ne "Disconnected"
    }
).IPv4Address.IPAddress
$env:DISPLAY = -join($env:DISPLAY, ":0.0")

echo "env:DISPLAY=$env:DISPLAY"

docker run -it --rm -e DISPLAY=$env:DISPLAY treeviewqobject:latest
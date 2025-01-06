$let = Read-Host "Type"

if ($let -eq 0) {
    git add -A
    $name = Read-Host "Name"
    git commit -m $name
    git push
} elseif ($let -eq 1) {
    Push-Location
    try {
        cd app_back_end
        go build
        .\head.exe
    } finally {
        Pop-Location
    }
}

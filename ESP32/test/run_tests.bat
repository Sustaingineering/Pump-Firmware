set remove="Writing\|Reading\|FILE\|Appending\|Deleting"
pio test --environment esp32doit-devkit-v1 | grep -i "test" | grep -v %remove% > test_results.txt

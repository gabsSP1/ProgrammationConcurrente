rm *.txt
ps -waux > ps_in.txt
ipcs > ipc_in.txt
./Parking
ps -waux > ps_out.txt
ipcs > ipc_out.txt
diff ps_in.txt ps_out.txt > ps_diff.txt
diff ipc_in.txt ipc_out.txt > ipc_diff.txt

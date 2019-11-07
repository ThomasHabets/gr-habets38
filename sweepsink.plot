set terminal png size 1920, 768
set output "wifi.png"
#1	2412	2401–2423	Yes	Yes	Yes
#2	2417	2406–2428	Yes	Yes	Yes
#3	2422	2411–2433	Yes	Yes	Yes
#4	2427	2416–2438	Yes	Yes	Yes
#5	2432	2421–2443	Yes	Yes	Yes
#6	2437	2426–2448	Yes	Yes	Yes
#7	2442	2431–2453	Yes	Yes	Yes
#8	2447	2436–2458	Yes	Yes	Yes
#9	2452	2441–2463	Yes	Yes	Yes
#10	2457	2446–2468	Yes	Yes	Yes
#11	2462	2451–2473	Yes	Yes	Yes
#12	2467	2456–2478	NoB except CAN	Yes	Yes
#13	2472	2461–2483	NoB	Yes	Yes
#14	2484	2473–2495
set object  1 rectangle from 2.401,0 to 2.423,100 fs solid fc rgb "#ffd0d0" behind
set object  6 rectangle from 2.426,0 to 2.448,100 fs solid fc rgb "#d0ffd0" behind
set object 11 rectangle from 2.451,0 to 2.473,100 fs solid fc rgb "#d0d0ff" behind
set format x "%.2fGHz"
plot [2.4:2.5] '~/test.csv' using ($2/1e9):3 w d title "Signal strength"

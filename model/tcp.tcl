set ns [new SImulator]

$ns color 1 Blue
$ns color 2 Red

set nf [open out.nam w]
$ns namtrace-all $nf 

proc finish{}{
    global ns nf
    $ns flush-trace
    close $nf
    exec nam out.nam &
    exit 0
}

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]

$ns duplex-link $n0 $n1 1Mb 10ms DropTail

$ns queu-limit $n0 $n1 10

$ns duplwx-link-op $n0 $n1 orient right

#tcp
set tcp [new Agent/TCP]
set sink [new Agent/TCPSink]
$ns attach-agent $n0 $tcp
$ns attach-agent $n2 $sink
$ns connect $tcp $sink

$tcp set packetSixe_ 10000
$tcp set fid_ 1
#tcp set classs_ 2

#ftp
set ftp [new Application/FTP]
$ns attach-agent $tcp $ftp
$tcp set type_ FTP
#udp
set udp [new Agent/UDP]
set null [new Agent/Null]
$ns attach-agent $n0 $udp
$ns attach-agent $n2 $null
$ns connect $udp $sink

#cbr
set cbr [new Application/Traffic/CBR]
$ns attach-agent $udp $cbr
$cbr set packetSize_ 10000

$ns at 1.0 "$ftp start"



define print_array
    if $argc != 4
        help print_array
    else
        set $fmt = $arg0
        set $base = $arg1
        set $step = $arg2
        set $count = $arg3

        set $i=0
        while ($i < $count)
            printf $fmt, *($base + $i*$step)
            $i++
        end
    end
end

document print_array
    usage: print_array <fmt> <base> <step> <count> 
end
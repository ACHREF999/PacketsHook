# PacketsHook
A Linux Driver using proc fs that logs the ips before routing (more like a template for network control)


So , what is this ? 
Well if you know that the kernel uses drivers to basically "implement" the syscalls , then this is simple ; 
this is a driver  , a piece of code the kernel "calls" when a certain ressource is requested for an operation (and since this is linux we are talking about they are file operations (if the kernel is before 5.6.0 then it is LITERALLY a struct called file_operations) ),
the driver is a piece of code that lives in the kernel space (built in if it is built in o: or dynamically inserted using insmod ... )
the interface that the user can use to talk to this driver is the /proc file system , 
this is a bare bones driver that does a task probably already handled by the kernel but it seemed intresting for me to do , 
the driver is subscribed to a netfilter hook , which helps us log the network request before routing , this can help build filters for the internet request or even collect data to LEARN :O user behaviour and so on ...

if u want to use this linux module u can use the command : 
```sudo insmod ldd.ko```

u can check that the module is up and running by using : 
```lsmod | grep ldd```


u can remove/eject the module using : 
```sudo rmmod ldd```


 


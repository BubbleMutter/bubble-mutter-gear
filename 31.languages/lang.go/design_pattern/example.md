[TOC]
# Code Come From
https://golangbyexample.com/
https://refactoring.guru/design-patterns

# creational
## abstract factory
1. product = shoe / shirt
2. factory = nike / adidas
3. steps:
    1. register all factory
    2. get specific factory to make product

## builder
1. builder = igloo / normal
2. director and house
3. steps:
    1. register all builder
    2. get specific builder
    3. director use builder to make house

## factory method
1. product = gun
2. factory = ak47 / musket
3. steps:
    1. register all gun factory
    2. get specific gun

## prototype
1. inode = file / folder
2. inode is the prototype for file and folder

## singeton

# structural
## adaptor
1. client   = insert sth into computer
2. computer = mac / windows / unix / ...
3. mac and windows are computer
    1. client insert to mac straightly
    2. however, client insert to windows via the adaptor
    3. because, windows does not implements the insert-method the computer

## bridge
1. computer = mac / windows / unix / ...
2. printer  = hp / epson / ...
3. computer has a printer
4. computer use printer to print
5. steps:
    1. get printer and computer
    2. set printer for computer

## composite
1. component = folder / file
2. folder includes components
3. method: tree-like method recursively perform to all components

## decorator
1. componet = pizza = veggieMania
2. decorator = topping  = tomatoTopping / cheeseTopping
3. method:
    1. use a decorator to decorate the componet
    2. inisde decorators' method, accumulate the component's method

## facade
1. demostrate how to build a big system with several complex subsystem

## flyweight

## proxy
1. request come through nginx, then is handled applicant.
2. nginx acts like a proxy in the middle station

# behavior
## chain of responsibility
1. a patient goes throught reception, doctor, medical and cashier
2. all department implenments "next" and "execute" method

## iterator
1. travel the abstract container
2. the specific container can be array, hash, tree and so on.

## memento

## state
1. state machine

## template_method
1. otp = email / sms
2. steps:
    1. otp execute several certain method
    2. but method is implement by the sms and email

## command

## meditor
1. meditor managle multi individual member
2. all member do not communicate with each other.

## observer
1. event-driven objects
2. subscribe and notify
3. steps:
    1. object notify all subscriber when updating event
    2. observer subscribe object
    3. observer get event when object notifying

## strategy
1. algo = fifo / lfu / lru
2. cache = use `algo` to effect the interactive way of the member

## visitor
1. obejct = circle / rectangle / square
2. visitor = coordinate / calculator
3. steps:
    1. object can be modified, changed or do anything else
    2. visitor get an object and analyze things it wanted,
       but without change the object

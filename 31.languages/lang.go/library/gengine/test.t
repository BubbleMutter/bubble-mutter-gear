rule "rule id1" "rule desc1" salience 10
BEGIN
    println(@id)
    println(@desc)
    println(@sal)
    return 0
END

rule "rule id2" "rule desc2" salience 10
BEGIN
    conc  {
        println("async thread")
    }
END

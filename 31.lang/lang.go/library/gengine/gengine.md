# gengine
https://github.com/bilibili/gengine

# main flow
```go
// allocate context
dataContext := context.NewDataContext()

// inject （variable or function)
dataContext.Add("Rs", Rs)
dataContext.Add("println", fmt.Println)

// allocate builder
ruleBuilder := builder.NewRuleBuilder(dataContext)
e1 := ruleBuilder.BuildRuleFromString(alias_rule)

// execute rule
eng := engine.NewGengine()
e := eng.Execute(ruleBuilder, true)

// retrive result ( return value in rule )
resultMap, err := eng.GetRulesResultMap()
```

# 13 Execute Mode
```go
sTag := &engine.Stag{StopTag: false}

e := gengine.Execute(ruleBuilder, false /* stop when error */)
e := gengine.ExecuteWithStopTagDirect(ruleBuilder, true /* continue even error */, sTag)

e := gengine.ExecuteMixModel(ruleBuilder)
e := gengine.ExecuteMixModelWithStopTagDirect(ruleBuilder, sTag)

e := gengine.ExecuteConcurrent(ruleBuilder)

e := gengine.ExecuteSelectedRules(ruleBuilder, []string{"rule id1", "rule id2"})
e := gengine.ExecuteSelectedRulesConcurrent(ruleBuilder, []string{"1", "2", "3", "4"})
e := gengine.ExecuteSelectedRulesMixModel(ruleBuilder, []string{"1", "2", "3", "4", "5"})
e := gengine.ExecuteSelectedRulesWithControl(ruleBuilder, false /* stop when error */ , []string{"1", "2", "3"})
e := gengine.ExecuteSelectedRulesWithControlAndStopTag(ruleBuilder, true, sTag, []string{"1", "2", "3", "4"})
e := gengine.ExecuteSelectedRulesWithControlAsGivenSortedName(ruleBuilder, true, []string{"3", "2", "1", "4"})
e := gengine.ExecuteSelectedRulesWithControlAndStopTagAsGivenSortedName(ruleBuilder, true, sTag3, []string{"2", "1", "4", "3"})
```
1. `Execute` 是顺序按优先级执行规则 （dsl 中的 salience)
2. `ExecuteMixModel` 是先执行最高优先级的执行, 剩下的全部并发执行
3. `ExecuteConcurrent` 全部规则并发执行
4. `ExecuteSelectedRules` 执行部分 rule
5. `*AndStopTag` 允许异步 goroutine 通过标记停止执行
6. `*AsGivenSortedName` 根据入参规则顺序执行 (即: 控制优先级)

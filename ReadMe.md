# 软件配置协议

## 加载性配置协议描述
加载性配置协议定义了一次性加载形态配置文件格式
定义：不限类型程序（程序内部配置或对进程配置） {启动时加载配置文件，不占用任何接口}
### 配置文件格式
```
<config-list pversion=“1.0”>
	<bool-indicator value="true(or false)">配置名称</bool-indicator>
	<number-input type="integer(or decimal)" limits="0:100(_:100或-100:_)" value="输入内容">配置名称</number-input>
	<text-input content="实际内容" [pre-content="默认内容"]>配置名称</text-input>
	<select-list selected-item="key-name" title="配置名称">
		<item key="key-name" name="选项名称" />
		<item key="key-name" name="选项名称" />
		<item key="key-name" name="选项名称" />
		<item key="key-name" name="选项名称" />
	</select-list>
	<font-config family="f-name" size="number" italic="true(or false)" strikeout="true(or false)" underline="true(or false)" bold="true(or false)">配置名称</font-config>
	<path-input type="file(or dir)" content="路径">配置名称</path-input>
	<color-input mode="rgb(or hsl,hsv,cmyk)" value="(v1,v2,v3,v4,v5)">配置名称</color-input>
	<items-group title="集合主题名称" title-editable="true(or false)">
		【摆放其他配置，集合性质】
		<template>
			【如果配置模板，允许遵照模板从配置端添加条目】
			【只允许单根项目，或者将组合项目利用group进行包装】
		</template>
		【实用建议：若确定布局的配置条目，无限制】
		【实用建议：若保存不定数量的配置，采用模板规定类型配置条目】
	</items-group>
	<optional-group title="可选配置集合主题名称" enable="true(or false)">
		【摆放其他配置，是否起作用需要判定】
	</optional-group>
	<set-stack actived-set="unique-key" title="选项卡集名称">
		<items-set key="unique-key" title="配置选项卡名称">
			【摆放其他配置项目】
		</items-set>
		<items-set key="unique-key" title="配置选项卡名称">
			【摆放其他配置项目】
		</items-set>
		<items-set key="unique-key" title="配置选项卡名称">
			【摆放其他配置项目】
		</items-set>
	</set-stack>
<config-list>
```




### 代码使用示例
```
ConfigV1 ftool;

```


## 交互性配置协议描述（暂讨论）
交互性配置协议定义了实时可交互性质配置协议格式
定义：进程级别配置工具针对典型应用程序 {需要占用标准输入输出，不占用api接口}

```
<config-list version="2.0">
	<text-input unique-key="unique-name" content="实际内容" [pre-content="默认内容"]>配置名称</text-input> 
	<number-input unique-key="unique-name" type="integer(or decimal)" limits="0:100(_:100或-100:_)" value="输入内容">配置名称</number-input>
	<switch unique-key="unique-name" select-item="key-name" title="配置名称">
		<item key="key-name" name="选项名称"/>
		<item key="key-name" name="选项名称"/>
		<item key="key-name" name="选项名称"/>
	</switch>
	<combobox unique-key="unique-name" select-item="key-name" title="配置名称">
		<item key="key-name" name="选项名称"/>
		<item key="key-name" name="选项名称"/>
		<item key="key-name" name="选项名称"/>
		<item key="key-name" name="选项名称"/>
	</combobox>
	<msg-table title="项目名称">
		<msg-line>
			<msg-unit></msg-unit>
			<msg-unit></msg-unit>
			<msg-unit></msg-unit>
			......
		</msg-line>
		</msg-line />
		</msg-line />
		</msg-line />
		......
	</msg-table>
</config-list>
```
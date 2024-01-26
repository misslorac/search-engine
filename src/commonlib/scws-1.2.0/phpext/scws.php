<?php
if(!extension_loaded('scws')) {
	dl('scws.' . PHP_SHLIB_SUFFIX);
}
$module = 'scws';
$functions = get_extension_funcs($module);
echo "Functions available in the test extension:<br>\n";
foreach($functions as $func) {
    echo $func."<br>\n";
}
echo "<br>\n";
$function = $module . '_version';
if (extension_loaded($module)) {
	$str = $function($module);
} else {
	$str = "Module $module is not compiled into PHP";
}
echo "$str\n\n";

$text = <<<EOF
�¿��貢���ǡ��޼�����Ψһ����Ȩ�ˣ�һ����Ӱ�������Ȩ���Ӱ��Ƭ�����С�

һ����Ӱ�����߰������ݡ���Ӱ�����ȴ�����Ա����Щ������Ա�����ǵĴ������а�Ȩ�ġ���������Ƭ����Ȩ�������˲��ܶԵ�Ӱ�����������С���ӳ������ͨ���������������Ȳ��ܰѵ�Ӱ�ı��С˵��������������������ʽ����Ҳ���ܰ�һ������Сʱ���ܷ���ĵ�Ӱ�ı�ɰ��Сʱ���ܷ���Ķ�Ƭ��

����Ȩ�Ͱ�Ȩ���ҹ���ͬһ������Ƿ��ɸ�����Ʒ�����ߵ�ר��Ȩ������νר��Ȩ������û�о���Ȩ��������ֲ��Ƿ��ɹ涨�����⣬Ҫʹ�������Ʒ���ͱ��뾭��������Ȩ��û����Ȩ������Ȩ��

һ�Ű����괺��
EOF;

$cws = scws_open();
scws_set_charset($cws, "gbk");
scws_set_dict($cws, ini_get('scws.default.fpath') . '/dict.xdb');
scws_set_rule($cws, ini_get('scws.default.fpath') . '/rules.ini');
//scws_set_ignore($cws, true);
//scws_set_multi($cws, true);
scws_send_text($cws, $text);

echo "<pre>\n";

// top words
printf("No. WordString               Attr  Weight(times)\n");
printf("-------------------------------------------------\n");
$list = scws_get_tops($cws, 10, "~v");
$cnt = 1;
foreach ($list as $tmp)
{
	printf("%02d. %-24.24s %-4.2s  %.2f(%d)\n",
		$cnt, $tmp['word'], $tmp['attr'], $tmp['weight'], $tmp['times']);
	$cnt++;
}

echo "\n\n-------------------------------------------------\n";
// segment
while ($res = scws_get_result($cws))
{
	foreach ($res as $tmp)
	{
		if ($tmp['len'] == 1 && $tmp['word'] == "\r")
			continue;
		if ($tmp['len'] == 1 && $tmp['word'] == "\n")
			echo $tmp['word'];
		else		
			printf("%s/%s ", $tmp['word'], $tmp['attr']);		
	}
}
echo "\n</pre>\n";

scws_close($cws);
?>

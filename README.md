# most-similar

## 元ネタ

["composer" のタイプミス多すぎ問題を解決する - Qiita](http://qiita.com/mpyw/items/a4495d476ea9ffe54e16)

## インストール

```
make && make install
```

もしパーミッションで怒られたら `sudo make install` で。

## 普通に使う

入力文字列に対して，候補の中から最も似ているものを探します。  
もし似ているものが無いと判断された場合，ステータス1で異常終了します。  
**`_`で始まるZshの補完用関数を無視します。**

### パラメータ

- **(必須)** `-i` で入力文字列を渡します。
- **(必須)** 標準入力で改行 `\n` 区切りにて候補を渡します。
- (任意) `-l` で候補の中からこの長さに満たないものを除外できます。
- (任意) `-s` で結果の中からこの類似度(**0.0 〜 1.0**の範囲)に満たないものを除外できます。

```ShellSession
mpyw@localhost:~$ ./most-similar -h
Usage: ./most-similar -i input < comparisons (Splitted by "\n")
  [-l min_length=1] [-s min_similarlity=0.0]

mpyw@localhost:~$ ./most-similar -i "compsoer" < <(echo "compress composer compset" | tr ' ' '\n')
composer
```

## Zshの誤り訂正として使う

`~/.zshrc` に以下を追記してください。

```zsh
command_not_found_handler() {

    local ___result
    ___result=$(
        print -l ${(ok)functions} ${(ok)aliases} ${(ok)commands} ${(ok)builtins} |
        /usr/local/bin/most-similar -i $0 -l 5 -s 0.6
    )

    if [[ $? == 0 ]]; then
        printf %s "Do you mean '$___result' ? [y/n]: "
        local answer
        read answer
        if [[ $answer == y || $answer == Y ]]; then
            als=$(alias $___result)
            if [[ $? == 0 ]]; then
                eval local $als
            fi
            shift
            eval $___result ${(q)@}
            return 
        fi
    fi

    return 127

}
```
# RayTracingInOneWeekend
手を動かしながら[Ray Tracing in One Weekend series](https://raytracing.github.io/)([日本語版](https://inzkyk.xyz/ray_tracing_in_one_weekend/))を学んでいる記録。

![](image.png)

# つまづいた点やメモ
(これissueでやることでは？)
## vec3クラスの実装
一部資料に`return ^this;`とある

- `return *this;`の誤字

## vec3, `inline std::ostream& operator<<`でコンパイルエラー
vec3.hの最後の行に`#endif`を書く

## 球体のレンダリング
赤い球が画面中心ではなく右上に配置される

- `vec3.h`の実装ミス
  - ×`operator/`, 〇`operator/=`
  - `operator*`: 乗算じゃなくて加算してた

## `[hittable.h] sphere`クラス
`hittable.h`に記述するのに`#include "hittable.h"`？

- 資料の誤字(pdf, HTML両方)
- 実際は`sphere.h`にsphereクラスを書く

## hittable_list
実装時に球体がレンダリングできなくなった。

- デバッグプリントで原因特定
  - `main` 球体衝突時のif文の中身が実行されていない
  - `hittable_list::hit` object->hitのif文がtrueにならない
  - `sphere::hit` discriminantは0以上だが、tempとt_max,t_minの比較結果がtrueにならない
  - t_max, t_minの値が異常に低い(6.95e-310, 0)
  - `hittable_list::hit` closest_so_far変数に`temp.rec.t`を入れて、それを`object->hit`のtmaxにしている。
    - ここだった。`auto closest_so_far = temp_rec.t;` => `auto closest_so_far = tmax;`

## `#include エラーが検出されました`
エラーをパッシブで可視化する拡張機能入れていたら`#include (自作ヘッダーファイル)`で上記エラー。includePathを更新してくれと言われる。

`${workplaceFolder}`をincludePathに追加して解決
```
            "includePath": [
                "${workspaceFolder}/**",
                "${workspaceFolder}"
            ],
```

## `ソース ファイルを開けません`(iostream, cmath, memoryなど)
`/usr/include/**`をincludePathに指定しても上記エラーが出る。

- includePathで`/usr/include/**`を設定すると、なぜか`(現在地)/usr/include/**`と同義になる。
- 原因不明。ひとまず無視

## ランバート反射に関して
BRDFを定義して、それに沿って実装しているのとは訳が違う

- 物体表面の法線から単位球を設定
- 新しくレイをサンプリングして方向を取得している
  - つまりランダムな方向にレイを飛ばして表現
- 再帰の部分で0.5かけてるので、反射1回につき光の半分が吸収されるものとみなしている。だから暗くなる？

## 完全なランバート反射
Scanlines remaining:87でsegmentation fault

- 前のランバート反射の実装のミス。再帰的呼び出しで`depth-1`を`depth`にしていた

## materialクラスの実装
日本語pdf資料にミス(`#include "rtweekend"`抜け)

## ランバーティアンの実装
日本語pdf資料の誤字`sphere.h` => `material.h`

## 金属球を使ったシーンの実装
コンパイルエラー。
```
./main.cpp:19:24: error: invalid use of incomplete type ‘using element_type = class material’ {aka ‘class material’}
   19 |         if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      |                        ^~
```

(日本語版・英語版どちらにも記載なし) `main.cc`に以下の2行を追加

```
#include "material.h"
#include "sphere.h"
```

# ぼやけた金属球
マイクロファセットじゃない実装。反射するレイの方向を、鏡面反射方向を中心とする球体のサンプルでとっている。BSDFで実装したときより変更点少なくてシンプル

## 誘電体の実装
日本語pdf資料の抜け：　`material.h`のメンバ`double ref_idx`の前に`public:`がない
- あと変数名を英語版から少し変えている

## 薄レンズ近似
サンプルレイはcam originを円盤状に広げて、そこからランダムにサンプルして実装する
- `random_in_unit_disk`：棄却法でサンプル

## 最後のシーン
Killedと表示される


```
hittable_list random_scene() {
    hittable_list world;
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    for (int a=-11; 1<11; a++) {
      ...
```
誤字で無限ループに陥ってた
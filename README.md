# RayTracingInOneWeekend
手を動かしながら[Ray Tracing in One Weekend series](https://raytracing.github.io/)([日本語版](https://inzkyk.xyz/ray_tracing_in_one_weekend/))を学んでいる記録。

# つまづいた点やメモ
## vec3クラスの実装
一部資料に`return ^this;`とある

- `return *this;`の誤字

## vec3, `inline std::ostream& operator<<`でコンパイルエラー
vec3.hの最後の行に`#endif`を書く

## 球体のレンダリング
赤い球が画面中心ではなく右上に配置される


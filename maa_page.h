const char PAGE_HTML[] PROGMEM = R"HTMLPAGE(
<!DOCTYPE HTML>
<html>
<head>
<meta charset="utf-8" />
<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no" />
<title>Enpty - MAA 控制台</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}

body{background:linear-gradient(-45deg,#a8c0ff,#d5a6f0,#ffb8d1,#b8a0ff);
  background-size:400% 400%;animation:gradientBG 20s ease infinite;
  color:#2a1f3d;font-family:-apple-system,BlinkMacSystemFont,"Segoe UI","PingFang SC","Microsoft YaHei",sans-serif;
  min-height:100vh;display:flex;align-items:center;justify-content:center;
  padding:2em 1em;
  overflow-x:hidden;overflow-y:auto;position:relative;}
@keyframes gradientBG{
  0%{background-position:0% 50%}
  50%{background-position:100% 50%}
  100%{background-position:0% 50%}
}

#bgLayer{position:fixed;inset:0;overflow:hidden;pointer-events:none;z-index:0}

.blob{position:absolute;border-radius:50%;filter:blur(70px);opacity:.55;will-change:transform}
.blob1{width:420px;height:420px;background:radial-gradient(circle,#ffb8d1,#d5a6f0);
  top:-12%;left:-10%;animation:blobFloat1 24s ease-in-out infinite}
.blob2{width:380px;height:380px;background:radial-gradient(circle,#a8c0ff,#b8a0ff);
  bottom:-15%;right:-10%;animation:blobFloat2 28s ease-in-out infinite}
.blob3{width:320px;height:320px;background:radial-gradient(circle,#ffe0f0,#ffb8d1);
  top:45%;right:12%;animation:blobFloat3 32s ease-in-out infinite}
.blob4{width:300px;height:300px;background:radial-gradient(circle,#c5d8ff,#a8c0ff);
  bottom:18%;left:8%;animation:blobFloat4 26s ease-in-out infinite}

@keyframes blobFloat1{0%,100%{transform:translate(0,0) scale(1)}33%{transform:translate(70px,50px) scale(1.15)}66%{transform:translate(30px,110px) scale(.9)}}
@keyframes blobFloat2{0%,100%{transform:translate(0,0) scale(1)}50%{transform:translate(-90px,-50px) scale(1.15)}}
@keyframes blobFloat3{0%,100%{transform:translate(0,0) scale(1)}50%{transform:translate(-70px,70px) scale(.85)}}
@keyframes blobFloat4{0%,100%{transform:translate(0,0) scale(1)}50%{transform:translate(80px,-40px) scale(1.1)}}

.petal{position:absolute;top:-30px;width:12px;height:12px;
  background:radial-gradient(circle at 30% 30%,#ffd1e8,#ff9ec7 70%);
  border-radius:50% 0 50% 0;
  box-shadow:0 0 6px rgba(255,158,199,.5);
  animation:petalFall linear infinite;opacity:.85;will-change:transform;pointer-events:none}
@keyframes petalFall{
  0%{transform:translateY(-30px) translateX(0) rotate(0deg);opacity:0}
  10%{opacity:.85}
  90%{opacity:.85}
  100%{transform:translateY(110vh) translateX(var(--drift,60px)) rotate(720deg);opacity:0}
}

.star{position:absolute;
  background:radial-gradient(circle,#fff 20%,rgba(255,255,255,0) 70%);
  border-radius:50%;pointer-events:none;
  animation:starTwinkle 3s ease-in-out infinite}
@keyframes starTwinkle{0%,100%{opacity:.15;transform:scale(.5)}50%{opacity:1;transform:scale(1.3)}}

.sym{position:absolute;font-size:14px;color:rgba(255,255,255,.85);
  text-shadow:0 0 8px rgba(255,184,209,.8);
  animation:symFloat linear infinite;pointer-events:none;will-change:transform,opacity}
@keyframes symFloat{
  0%{transform:translateY(0) scale(.6) rotate(0deg);opacity:0}
  15%{opacity:.9}
  85%{opacity:.9}
  100%{transform:translateY(-100vh) scale(1.2) rotate(360deg);opacity:0}
}

.loader{position:fixed;inset:0;
  background:linear-gradient(-45deg,#a8c0ff,#d5a6f0,#ffb8d1,#b8a0ff);
  background-size:400% 400%;animation:gradientBG 20s ease infinite;
  display:flex;align-items:center;justify-content:center;flex-direction:column;
  z-index:9999;transition:opacity .8s ease,visibility .8s}
.loader.hide{opacity:0;visibility:hidden;pointer-events:none}
.loader-wrap{position:relative;width:100px;height:150px;margin-bottom:24px}
.lemon-emoji{position:absolute;top:0;left:50%;
  transform:translateX(-50%);font-size:2.6em;line-height:1;
  animation:lemonSqueeze 1.4s ease-in-out infinite;z-index:2;
  filter:drop-shadow(0 4px 10px rgba(0,0,0,.15))}
@keyframes lemonSqueeze{
  0%,100%{transform:translateX(-50%) rotate(-14deg)}
  25%{transform:translateX(-50%) rotate(12deg)}
  50%{transform:translateX(-50%) rotate(-14deg)}
  75%{transform:translateX(-50%) rotate(12deg)}
}
.cup{position:absolute;bottom:0;left:50%;transform:translateX(-50%);
  width:80px;height:96px;
  border:2px solid rgba(255,255,255,.9);border-top:none;
  border-radius:6px 6px 34px 34px;
  background:linear-gradient(180deg,rgba(255,255,255,.3),rgba(255,255,255,.08));
  backdrop-filter:blur(8px);-webkit-backdrop-filter:blur(8px);
  overflow:hidden;
  box-shadow:inset 0 -8px 20px rgba(0,0,0,.08),
    inset 2px 0 6px rgba(255,255,255,.4),
    inset -2px 0 6px rgba(255,255,255,.4),
    0 12px 32px rgba(168,192,255,.4)}
.cup::before{content:'';position:absolute;top:0;left:8px;width:6px;height:100%;
  background:linear-gradient(180deg,rgba(255,255,255,.65),transparent 75%);
  border-radius:3px;pointer-events:none;z-index:3}
.liquid{position:absolute;bottom:0;left:0;right:0;height:0%;
  background:linear-gradient(180deg,#ffe066,#f7c948,#f0b400);
  border-radius:0 0 32px 32px;
  animation:liquidFill 1.4s ease-in-out infinite;
  box-shadow:0 -6px 16px rgba(247,201,72,.55)}
.liquid::before{content:'';position:absolute;top:-5px;left:0;right:0;height:10px;
  background:radial-gradient(ellipse at center,rgba(255,255,255,.7) 0%,rgba(255,224,102,.4) 55%,transparent 85%);
  border-radius:50%}
@keyframes liquidFill{0%{height:0%}60%{height:74%}85%{height:74%}100%{height:0%}}
.loader-text{color:#fff;font-size:1.1em;letter-spacing:.35em;margin-bottom:.6em;
  text-shadow:0 2px 10px rgba(168,192,255,.5)}
.loader-sub{color:rgba(255,255,255,.75);font-size:.9em;letter-spacing:.45em}

#wrapper{width:100%;max-width:29em;position:relative;z-index:2;margin:auto;margin-top:20px;margin-bottom:20px;}

/* ========== 左下角 Q 版小猫 ========== */
.chibi{position:fixed;bottom:16px;left:16px;
  width:110px;height:110px;z-index:10;pointer-events:none;
  animation:chibiFloat 3.5s ease-in-out infinite;
  filter:drop-shadow(0 6px 16px rgba(168,192,255,.5))}
.chibi svg{width:100%;height:100%;display:block;overflow:visible}
@keyframes chibiFloat{
  0%,100%{transform:translateY(0)}
  50%{transform:translateY(-8px)}
}

/* 气泡 */
.chibi-bubble{position:fixed;bottom:110px;left:80px;
  background:rgba(255,255,255,.95);
  color:#5a7fc8;font-size:11px;font-weight:600;
  padding:6px 12px;border-radius:12px;white-space:nowrap;
  box-shadow:0 4px 12px rgba(168,192,255,.4),
    inset 0 1px 0 rgba(255,255,255,1);
  opacity:0;animation:bubblePop 4s ease-in-out infinite;
  pointer-events:none;z-index:11}
.chibi-bubble::before{content:'';position:absolute;
  left:12px;bottom:-5px;
  width:0;height:0;border-left:5px solid transparent;border-right:5px solid transparent;
  border-top:6px solid rgba(255,255,255,.95)}
@keyframes bubblePop{
  0%,40%{opacity:0;transform:scale(.6) translateY(5px)}
  50%,80%{opacity:1;transform:scale(1) translateY(0)}
  90%,100%{opacity:0;transform:scale(.6) translateY(5px)}
}

/* 小猫眨眼 */
.chibi .eye-l,.chibi .eye-r{transform-origin:center;animation:blink 4s ease-in-out infinite}
@keyframes blink{
  0%,90%,100%{transform:scaleY(1)}
  94%,96%{transform:scaleY(.1)}
}

/* ========== 磨砂白卡片 ========== */
#main{position:relative;
  background:linear-gradient(135deg,
    rgba(255,245,252,.6) 0%,
    rgba(255,255,255,.42) 45%,
    rgba(245,240,255,.55) 100%);
  backdrop-filter:blur(40px) saturate(180%) brightness(1.05);
  -webkit-backdrop-filter:blur(40px) saturate(180%) brightness(1.05);
  border:2px solid rgba(255,255,255,.85);
  border-radius:28px;
  padding:3.5em 2.5em 3em;
  text-align:center;
  box-shadow:
    0 24px 60px rgba(120,90,180,.28),
    0 8px 20px rgba(120,90,180,.15),
    inset 0 2px 4px rgba(255,255,255,1),
    inset 0 -2px 4px rgba(255,255,255,.5),
    inset 2px 0 4px rgba(255,255,255,.8),
    inset -2px 0 4px rgba(255,255,255,.8);
  transition:transform 1s cubic-bezier(.4,0,.2,1);
  overflow:hidden}

#main::before{content:'';position:absolute;top:0;left:8%;right:8%;height:35%;
  background:radial-gradient(ellipse at top,rgba(255,255,255,.85),transparent 70%);
  border-radius:28px 28px 50% 50%;pointer-events:none;z-index:1}
#main::after{content:'';position:absolute;bottom:0;left:0;right:0;height:35%;
  background:linear-gradient(0deg,rgba(168,192,255,.15),transparent);
  border-radius:0 0 28px 28px;pointer-events:none;z-index:1}

.corner-deco{position:absolute;font-size:16px;color:rgba(168,192,255,.6);
  z-index:3;pointer-events:none;user-select:none;
  text-shadow:0 0 8px rgba(168,192,255,.8)}
.corner-deco.tl{top:12px;left:16px}
.corner-deco.tr{top:12px;right:16px}
.corner-deco.bl{bottom:12px;left:16px}
.corner-deco.br{bottom:12px;right:16px}

#main>header{transition:opacity .5s ease;position:relative;z-index:2}
.hide-v{display:none!important}

.conn{position:absolute;top:1em;right:1.2em;display:flex;align-items:center;gap:.35em;
  font-size:.68em;color:rgba(42,31,61,.6);letter-spacing:.1em;z-index:4;font-weight:600}
.conn .dot{width:7px;height:7px;border-radius:50%;background:#888;transition:background .3s}
.conn.online .dot{background:#10b981;box-shadow:0 0 8px rgba(16,185,129,.7)}
.conn.offline .dot{background:#ef4444;box-shadow:0 0 8px rgba(239,68,68,.7)}

.avatar{width:100px;height:100px;margin:0 auto 1em;border-radius:50%;
  overflow:hidden;
  box-shadow:
    0 10px 26px rgba(168,192,255,.5),
    inset 0 2px 8px rgba(255,255,255,.5);
  transition:transform .3s;
  background:linear-gradient(135deg,#a8c0ff,#ffb8d1);
  display:block;border:3px solid rgba(255,255,255,.95);
  position:relative}
.avatar:hover{transform:scale(1.05) rotate(-3deg)}
.avatar img{width:100%;height:100%;object-fit:cover;display:block}

h1{font-size:1.6em;font-weight:700;letter-spacing:.12em;margin-bottom:.5em;
  color:#2a1f3d;font-family:"Segoe UI",sans-serif}
p{color:rgba(42,31,61,.72);line-height:1.8;font-size:.9em;margin-bottom:.8em}
.hint{font-size:.78em;color:rgba(42,31,61,.55);letter-spacing:.1em}

.button{display:inline-block;padding:.8em 1.4em;border-radius:12px;
  border:1.5px solid rgba(255,255,255,.9);
  background:rgba(255,255,255,.55);
  color:#2a1f3d;font-size:.85em;cursor:pointer;
  transition:all .3s;letter-spacing:.1em;
  text-decoration:none;font-family:inherit;font-weight:600;
  position:relative;overflow:hidden;
  backdrop-filter:blur(12px) saturate(150%);
  -webkit-backdrop-filter:blur(12px) saturate(150%);
  box-shadow:
    0 4px 12px rgba(120,90,180,.12),
    inset 0 1.5px 0 rgba(255,255,255,1),
    inset 0 -1px 0 rgba(255,255,255,.4)}
.button:hover{background:rgba(255,255,255,.8);
  transform:translateY(-2px);
  box-shadow:0 8px 20px rgba(120,90,180,.22),
    inset 0 1.5px 0 rgba(255,255,255,1)}
.button.fit{display:block;width:100%;margin:.8em 0 .4em 0;}
.button.small{padding:.5em 1.1em;font-size:.7em;border-radius:10px}
.button.on{background:linear-gradient(135deg,#a8c0ff,#d5a6f0);
  border:1.5px solid rgba(255,255,255,.6);color:#fff;font-weight:700;
  box-shadow:0 6px 20px rgba(168,192,255,.5),
    inset 0 1.5px 0 rgba(255,255,255,.6)}
.button.off{background:linear-gradient(135deg,#ffb8d1,#d5a6f0);
  border:1.5px solid rgba(255,255,255,.6);color:#fff;font-weight:700;
  box-shadow:0 6px 20px rgba(255,184,209,.5),
    inset 0 1.5px 0 rgba(255,255,255,.6)}
.button.on:hover,.button.off:hover{transform:translateY(-3px) scale(1.02)}
.button:active{transform:translateY(0) scale(.98);opacity:.9}
.button:disabled{opacity:.4;cursor:not-allowed;transform:none!important}

hr{border:0;border-top:1px solid rgba(255,255,255,.8);margin:1.4em 0}

.input{width:100%;padding:1em 1.2em;margin-bottom:1em;border-radius:12px;
  border:1.5px solid rgba(255,255,255,.9);
  background:rgba(255,255,255,.6);
  color:#2a1f3d;font-size:.9em;font-family:inherit;letter-spacing:.05em;
  outline:none;transition:all .3s;
  backdrop-filter:blur(12px);-webkit-backdrop-filter:blur(12px);
  box-shadow:inset 0 1.5px 0 rgba(255,255,255,1),
    inset 0 -1px 0 rgba(255,255,255,.4)}
.input:focus{border-color:rgba(213,166,240,.9);
  background:rgba(255,255,255,.85);
  box-shadow:0 0 0 4px rgba(213,166,240,.2),
    inset 0 1.5px 0 rgba(255,255,255,1)}
.input::placeholder{color:rgba(42,31,61,.4)}

.status{padding:.85em;border-radius:12px;margin:.9em 0;
  font-size:.85em;letter-spacing:.12em;font-weight:600;
  transition:all .4s;position:relative;
  backdrop-filter:blur(10px);-webkit-backdrop-filter:blur(10px)}
.status.on{background:rgba(16,185,129,.2);color:#0a7a52;
  border:1.5px solid rgba(16,185,129,.5);
  animation:breathe 2s ease-in-out infinite}
.status.off{background:rgba(239,68,68,.14);color:#b91c3e;
  border:1.5px solid rgba(239,68,68,.4)}
@keyframes breathe{0%,100%{box-shadow:0 0 0 rgba(16,185,129,0)}50%{box-shadow:0 0 24px rgba(16,185,129,.4)}}

.log{text-align:left;padding:.85em;border-radius:12px;
  background:rgba(255,255,255,.6);
  border:1.5px solid rgba(255,255,255,.9);
  font-family:"SF Mono",Menlo,Consolas,monospace;font-size:.7em;line-height:1.8;
  color:rgba(42,31,61,.8);
  white-space:pre-wrap;word-break:break-all;min-height:80px;
  max-height:160px;overflow-y:auto;margin:.7em 0;
  backdrop-filter:blur(10px);-webkit-backdrop-filter:blur(10px);
  box-shadow:inset 0 1.5px 0 rgba(255,255,255,1),
    inset 0 -1px 0 rgba(255,255,255,.4)}
.log::-webkit-scrollbar{width:4px}
.log::-webkit-scrollbar-thumb{background:rgba(213,166,240,.5);border-radius:2px}

.id-card{padding:.85em;border-radius:12px;margin-bottom:.8em;text-align:left;
  background:rgba(213,166,240,.18);
  border:1.5px solid rgba(255,255,255,.8);
  backdrop-filter:blur(12px);
  transition:all .3s;
  box-shadow:inset 0 1.5px 0 rgba(255,255,255,.9),
    inset 0 -1px 0 rgba(255,255,255,.4)}
.id-card:hover{background:rgba(213,166,240,.3);
  border-color:rgba(255,255,255,1)}
.id-card .label{font-size:.7em;color:#8a5fc8;
  letter-spacing:.12em;margin-bottom:.4em;font-weight:700}
.id-val{font-family:"SF Mono",Menlo,monospace;font-size:.8em;color:#2a1f3d;
  background:rgba(255,255,255,.8);padding:.6em;border-radius:10px;
  word-break:break-all;user-select:all;margin-bottom:.5em;
  border:1px solid rgba(255,255,255,.9);
  box-shadow:inset 0 1px 0 rgba(255,255,255,1)}

#footer{text-align:center;margin-top:1.5em}
.copyright{list-style:none}
.copyright li{font-size:.68em;color:rgba(255,255,255,.9);
  letter-spacing:.1em;line-height:2;
  text-shadow:0 1px 4px rgba(120,90,180,.3)}
.copyright a{color:rgba(255,255,255,.9);text-decoration:none}
.copyright a:hover{color:#fff}

.swap-anim{background-image:linear-gradient(-45deg,#2a1f3d 40%,#a86fd8 50%,#2a1f3d 60%);
  background-clip:text;-webkit-background-clip:text;-webkit-text-fill-color:transparent;
  background-size:300%;animation:swap 4s linear infinite;display:inline-block}
@keyframes swap{0%{background-position:300%}50%{background-position:0%}100%{background-position:300%}}

.toast{position:fixed;bottom:2em;left:50%;
  transform:translateX(-50%) translateY(120px);
  background:linear-gradient(135deg,#a8c0ff,#d5a6f0);color:#fff;
  padding:.7em 1.5em;border-radius:14px;
  font-size:.8em;font-weight:600;letter-spacing:.1em;
  transition:transform .4s,opacity .4s;z-index:99999;
  max-width:90vw;text-align:center;
  opacity:0;pointer-events:none;
  box-shadow:0 8px 24px rgba(168,192,255,.5),
    inset 0 1.5px 0 rgba(255,255,255,.6)}
.toast.show{transform:translateX(-50%) translateY(0);opacity:1}

@media (max-width:480px){
  #main{padding:2.5em 1.5em 2em;border-radius:22px}
  h1{font-size:1.4em}
  .button{font-size:.8em;padding:.7em 1.2em}
  .blob{filter:blur(40px)}
  .chibi{width:80px;height:80px;bottom:10px;left:10px}
  .chibi-bubble{bottom:95px;left:70px;font-size:10px;padding:5px 10px}
}
</style>
</head>
<body class="is-preload">

<div id="bgLayer">
  <div class="blob blob1"></div>
  <div class="blob blob2"></div>
  <div class="blob blob3"></div>
  <div class="blob blob4"></div>
</div>

<div class="loader" id="loader">
  <div class="loader-wrap">
    <div class="lemon-emoji">🍋</div>
    <div class="cup"><div class="liquid"></div></div>
  </div>
  <p class="loader-text">榨汁</p>
  <p class="loader-sub">再喝</p>
</div>

<!-- 左下角 Q 版小猫 -->
<div class="chibi">
  <svg viewBox="0 0 120 120" xmlns="http://www.w3.org/2000/svg">
    <!-- ===== 身体/脖子（简单线条） ===== -->
    <path d="M 42 90 L 38 118 M 78 90 L 82 118"
      stroke="#1a1a2e" stroke-width="3" stroke-linecap="round"/>

    <!-- ===== 耳朵 ===== -->
    <path d="M 28 30 L 18 6 Q 35 12 48 22"
      fill="#e8ecf1" stroke="#1a1a2e" stroke-width="3" stroke-linejoin="round"/>
    <path d="M 92 30 L 102 6 Q 85 12 72 22"
      fill="#e8ecf1" stroke="#1a1a2e" stroke-width="3" stroke-linejoin="round"/>

    <!-- ===== 头部（带点方形的圆） ===== -->
    <path d="M 26 35 Q 26 15 60 15 Q 94 15 94 35 Q 94 85 85 91 Q 60 97 35 91 Q 26 85 26 35 Z"
      fill="#e8ecf1" stroke="#1a1a2e" stroke-width="3" stroke-linejoin="round"/>

    <!-- ===== 左眼（大黑眼珠 + 黄色边缘）===== -->
    <g class="eye-l">
      <circle cx="44" cy="56" r="23" fill="#ffe066" stroke="#1a1a2e" stroke-width="3"/>
      <circle cx="44" cy="56" r="19" fill="#1a1a2e"/>
      <circle cx="37" cy="48" r="6.5" fill="#fff"/>
      <circle cx="50" cy="68" r="2.5" fill="#fff" opacity=".85"/>
    </g>

    <!-- ===== 右眼 ===== -->
    <g class="eye-r">
      <circle cx="76" cy="56" r="23" fill="#ffe066" stroke="#1a1a2e" stroke-width="3"/>
      <circle cx="76" cy="56" r="19" fill="#1a1a2e"/>
      <circle cx="69" cy="48" r="6.5" fill="#fff"/>
      <circle cx="82" cy="68" r="2.5" fill="#fff" opacity=".85"/>
    </g>

    <!-- ===== 嘴巴（小短横） ===== -->
    <path d="M 56 84 L 64 84" stroke="#1a1a2e" stroke-width="2.5" stroke-linecap="round"/>

    <!-- ===== 装饰 ===== -->
    <text x="104" y="98" font-size="10" fill="#4fc3f7" opacity=".85">✦</text>
    <text x="6" y="104" font-size="9" fill="#a8c0ff" opacity=".85">✧</text>
  </svg>
</div>

<!-- 气泡 -->
<div class="chibi-bubble">欢迎回来~</div>

<div id="wrapper">
  <section id="main">
    <span class="corner-deco tl">✿</span>
    <span class="corner-deco tr">✦</span>
    <span class="corner-deco bl">✦</span>
    <span class="corner-deco br">✿</span>

    <header id="loginView" class="hide-v">
      <h1 class="swap-anim">MAA 控制台</h1>
      <p class="hint" style="margin-bottom:2em">请输入用户名和密码</p>
      <input type="text" class="input" id="username" placeholder="用户名" autocomplete="username" />
      <input type="password" class="input" id="password" placeholder="密码" autocomplete="current-password" />
      <button class="button fit on" onclick="doLogin()" id="loginBtn">登 录</button>
    </header>

    <header id="maaView" class="hide-v">
      <div class="conn" id="connState"><span class="dot"></span><span id="connText">连接中</span></div>
      <span class="avatar"><img src="/avatar.jpg" alt="avatar" onerror="this.style.display='none';this.parentNode.textContent='E';this.parentNode.style.lineHeight='100px';this.parentNode.style.fontSize='2.5em';this.parentNode.style.display='flex';this.parentNode.style.alignItems='center';this.parentNode.style.justifyContent='center'" /></span>
      <h1>MAA 控制台</h1>
      <p class="hint" id="welcomeUser" style="margin-bottom:1em"></p>

      <div class="id-card">
        <div class="label">📋 你的 MAA 用户标识符</div>
        <div class="id-val" id="maaUser">加载中...</div>
        <button class="button small fit" onclick="copyId()" id="copyBtn">复制标识符</button>
      </div>

      <!-- 新增的端点卡片 -->
      <div class="id-card">
        <div class="label">📡 远程控制端点</div>
        <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:.4em;">
          <span style="font-size:.75em; color:#8a5fc8; white-space:nowrap;">获取任务</span>
          <span class="id-val" style="margin-bottom:0; padding:.4em .6em; font-size:.7em; flex:1; margin-left:.8em; text-align:left; overflow-x:auto; white-space:nowrap;" id="endpointGet">加载中...</span>
        </div>
        <div style="display:flex; justify-content:space-between; align-items:center;">
          <span style="font-size:.75em; color:#8a5fc8; white-space:nowrap;">汇报任务</span>
          <span class="id-val" style="margin-bottom:0; padding:.4em .6em; font-size:.7em; flex:1; margin-left:.8em; text-align:left; overflow-x:auto; white-space:nowrap;" id="endpointReport">加载中...</span>
        </div>
        <button class="button small fit" style="margin-top:.8em;" onclick="copyEndpoint()" id="copyEndpointBtn">一键复制端点</button>
      </div>

      <div id="status" class="status off">● 状态加载中...</div>

      <button class="button fit on" onclick="setState('on', this)" id="btnOn">▶ 开启一键长草</button>
      <button class="button fit off" onclick="setState('off', this)" id="btnOff">■ 停止当前任务</button>

      <div class="log" id="log">日志加载中...</div>

      <hr />
      <div id="adminLink" class="hide-v" style="margin-bottom:.6em">
        <button class="button small fit" onclick="toAdmin()">⚙ 后台管理</button>
      </div>
      <button class="button small fit" onclick="doLogout()">退出登录</button>
    </header>

    <header id="adminView" class="hide-v">
      <h1>⚙ 后台</h1>
      <p class="hint" style="margin-bottom:1.2em">系统信息与用户状态</p>

      <div class="log" id="adminInfo" style="text-align:left">加载中...</div>

      <hr />
      <button class="button small fit" onclick="backToMaa()">返回控制台</button>
      <button class="button small fit" onclick="doLogout()">退出登录</button>
    </header>

  </section>

  <footer id="footer">
    <ul class="copyright">
      <li></li>
      <li><a href="https://beian.miit.gov.cn/" target="_blank">沪ICP备2024049069号-1</a></li>
    </ul>
  </footer>
</div>

<div class="toast" id="toast"></div>

<script>
(function initBackground() {
  var bg = document.getElementById('bgLayer');
  var petalCount = 20;
  for (var i = 0; i < petalCount; i++) {
    (function() {
      var p = document.createElement('div');
      p.className = 'petal';
      var size = 8 + Math.random() * 8;
      p.style.width = size + 'px';
      p.style.height = size + 'px';
      p.style.left = Math.random() * 100 + '%';
      p.style.animationDuration = (8 + Math.random() * 10) + 's';
      p.style.animationDelay = (Math.random() * 12) + 's';
      p.style.setProperty('--drift', ((Math.random() - 0.5) * 200) + 'px');
      if (Math.random() > 0.6) p.style.opacity = 0.55;
      bg.appendChild(p);
    })();
  }
  for (var j = 0; j < 40; j++) {
    var s = document.createElement('div');
    s.className = 'star';
    var sw = Math.random() * 4 + 2;
    s.style.width = sw + 'px';
    s.style.height = sw + 'px';
    s.style.left = Math.random() * 100 + '%';
    s.style.top = Math.random() * 100 + '%';
    s.style.animationDelay = (Math.random() * 3) + 's';
    s.style.animationDuration = (2 + Math.random() * 3) + 's';
    bg.appendChild(s);
  }
  var symbols = ['✦','✧','♡','✿','❀','⋆','★','☆'];
  for (var k = 0; k < 14; k++) {
    var sym = document.createElement('div');
    sym.className = 'sym';
    sym.textContent = symbols[Math.floor(Math.random() * symbols.length)];
    sym.style.left = Math.random() * 100 + '%';
    sym.style.bottom = '-30px';
    sym.style.fontSize = (10 + Math.random() * 14) + 'px';
    sym.style.animationDuration = (12 + Math.random() * 14) + 's';
    sym.style.animationDelay = (Math.random() * 15) + 's';
    var colors = ['rgba(255,184,209,.9)','rgba(213,166,240,.9)','rgba(168,192,255,.9)','rgba(255,255,255,.9)'];
    sym.style.color = colors[Math.floor(Math.random() * colors.length)];
    bg.appendChild(sym);
  }
})();

function getToken() { return localStorage.getItem('maa_token') || ''; }
function setToken(t) { localStorage.setItem('maa_token', t); }
function clearToken() { localStorage.removeItem('maa_token'); }
function getUsername() { return localStorage.getItem('maa_username') || ''; }
function setUsername(u) { localStorage.setItem('maa_username', u); }

var connFailCount = 0;
function setConnState(online) {
  var el = document.getElementById('connState');
  var txt = document.getElementById('connText');
  if (online) { el.className = 'conn online'; txt.textContent = '已连接'; }
  else { el.className = 'conn offline'; txt.textContent = '离线'; }
}

function showView(viewId) {
  var ids = ['loginView', 'maaView', 'adminView'];
  for (var i = 0; i < ids.length; i++) {
    var el = document.getElementById(ids[i]);
    if (el) {
      if (ids[i] === viewId) el.classList.remove('hide-v');
      else el.classList.add('hide-v');
    }
  }
}

var deg = 0;
function rotateCard(d) {
  var ele = document.getElementById('main');
  if (!ele) return;
  deg += d;
  ele.style.transition = 'transform 1s cubic-bezier(.4,0,.2,1)';
  ele.style.transform = 'rotateY(' + deg + 'deg)';
  setTimeout(function() {
    if (deg >= 360 || deg <= -360) {
      deg = 0;
      ele.style.transition = 'none';
      ele.style.transform = 'rotateY(0deg)';
      setTimeout(function() {
        ele.style.transition = 'transform 1s cubic-bezier(.4,0,.2,1)';
      }, 50);
    }
  }, 1000);
}

function toAdmin() { showView('adminView'); rotateCard(-360); loadAdmin(); }
function backToMaa() { showView('maaView'); rotateCard(360); }

function toast(msg) {
  var t = document.getElementById('toast');
  t.textContent = msg;
  if (t._hideTimer) { clearTimeout(t._hideTimer); t._hideTimer = null; }
  void t.offsetWidth;
  t.classList.add('show');
  t._hideTimer = setTimeout(function() {
    t.classList.remove('show');
    t._hideTimer = null;
  }, 2000);
}

function copyId() {
  var text = document.getElementById('maaUser').textContent.trim();
  if (!text || text === '加载中...') return;
  var done = function() {
    var btn = document.getElementById('copyBtn');
    var original = btn.textContent;
    btn.textContent = '✓ 已复制';
    setTimeout(function() { btn.textContent = original; }, 2000);
  };
  if (navigator.clipboard) {
    navigator.clipboard.writeText(text).then(done).catch(function() { fallbackCopy(text); done(); });
  } else { fallbackCopy(text); done(); }
}

function copyEndpoint() {
  var url1 = document.getElementById('endpointGet').textContent;
  var url2 = document.getElementById('endpointReport').textContent;
  if (url1 === '加载中...' || url2 === '加载中...') return;
  
  var text = "获取任务端点: " + url1 + "\n汇报任务端点: " + url2;
  
  var done = function() {
    var btn = document.getElementById('copyEndpointBtn');
    var original = btn.textContent;
    btn.textContent = '✓ 已复制';
    setTimeout(function() { btn.textContent = original; }, 2000);
  };
  
  if (navigator.clipboard) {
    navigator.clipboard.writeText(text).then(done).catch(function() { fallbackCopy(text); done(); });
  } else { 
    fallbackCopy(text); 
    done(); 
  }
}

function fallbackCopy(text) {
  var ta = document.createElement('textarea');
  ta.value = text; document.body.appendChild(ta); ta.select();
  document.execCommand('copy'); document.body.removeChild(ta);
  toast('已复制');
}

function doLogin() {
  var u = document.getElementById('username').value.trim();
  var p = document.getElementById('password').value;
  if (!u || !p) { toast('请输入用户名和密码'); return; }
  var btn = document.getElementById('loginBtn');
  btn.disabled = true;
  btn.textContent = '登录中...';
  fetch('/login', {
    method: 'POST',
    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
    body: 'username=' + encodeURIComponent(u) + '&password=' + encodeURIComponent(p)
  }).then(function(r) {
    if (r.status === 200) {
      return r.text().then(function(token) {
        token = token.trim();
        if (!token) { toast('登录失败：服务器未返回 token'); return; }
        setToken(token); setUsername(u);
        showView('maaView'); rotateCard(-360);
        initControl(u);
        toast('登录成功');
      });
    } else {
      toast('登录失败：用户名或密码错误');
    }
  }).catch(function(err) { toast('网络错误: ' + err); })
    .finally(function() {
      btn.disabled = false;
      btn.textContent = '登 录';
    });
}

function doLogout() {
  var token = getToken();
  fetch('/logout?token=' + encodeURIComponent(token)).finally(function() {
    clearToken();
    if (stateTimer) { clearInterval(stateTimer); stateTimer = null; }
    document.getElementById('username').value = '';
    document.getElementById('password').value = '';
    showView('loginView'); rotateCard(360);
  });
}

var stateTimer = null;
function initControl(username) {
  document.getElementById('welcomeUser').textContent = '👤 ' + username;
  loadState();
  if (stateTimer) clearInterval(stateTimer);
  stateTimer = setInterval(loadState, 2000);
}

function loadState() {
  var token = getToken();
  if (!token) { showView('loginView'); return; }
  fetch('/maa/state?token=' + encodeURIComponent(token))
    .then(function(r) {
      if (r.status === 401) {
        clearToken();
        if (stateTimer) { clearInterval(stateTimer); stateTimer = null; }
        showView('loginView'); toast('会话已过期，请重新登录');
        return null;
      }
      return r.json();
    })
    .then(function(d) {
      if (!d) return;
      connFailCount = 0; setConnState(true);
      var el = document.getElementById('status');
      if (d.on) { el.className = 'status on'; el.textContent = '● 已开启 · 命令已下发'; }
      else { el.className = 'status off'; el.textContent = '○ 已关闭'; }
      if (d.maaUser) document.getElementById('maaUser').textContent = d.maaUser;
      if (d.username) document.getElementById('welcomeUser').textContent = '👤 ' + d.username;
      if (d.isAdmin) document.getElementById('adminLink').classList.remove('hide-v');
      
      if (d.ip) {
        var baseUrl = 'http://' + d.ip;
        document.getElementById('endpointGet').textContent = baseUrl + '/maa/getTask';
        document.getElementById('endpointReport').textContent = baseUrl + '/maa/reportStatus';
      }
      
      var lg = document.getElementById('log');
      var txt = '';
      if (d.log1) txt += d.log1 + '\n';
      if (d.log2) txt += d.log2 + '\n';
      if (d.log3) txt += d.log3 + '\n';
      var newText = txt || '暂无日志';
      if (lg.textContent !== newText) {
        lg.textContent = newText;
        lg.scrollTop = lg.scrollHeight;
      }
    })
    .catch(function(err) {
      connFailCount++;
      if (connFailCount >= 3) setConnState(false);
      console.error('loadState 错误:', err);
    });
}

function setState(s, btn) {
  var token = getToken();
  if (!token) { showView('loginView'); return; }
  var origText = btn.textContent;
  btn.disabled = true;
  btn.textContent = '下发中...';
  fetch('/maa/set?state=' + s + '&token=' + encodeURIComponent(token))
    .then(function(r) {
      if (r.status === 401) {
        clearToken();
        if (stateTimer) { clearInterval(stateTimer); stateTimer = null; }
        showView('loginView');
        return null;
      }
      return r.text();
    })
    .then(function(t) {
      if (t !== null) {
        toast(s === 'on' ? '已下发：开启' : '已下发：停止');
        loadState();
      }
    })
    .catch(function(e) { toast('操作失败: ' + e); })
    .finally(function() {
      setTimeout(function() { btn.disabled = false; btn.textContent = origText; }, 800);
    });
}

function loadAdmin() {
  var token = getToken();
  fetch('/admin/data?token=' + encodeURIComponent(token))
    .then(function(r) { return r.json(); })
    .then(function(d) {
      var txt = '';
      txt += '📡 设备信息\n';
      txt += '  IP 地址   ' + d.ip + '\n';
      txt += '  运行时间  ' + d.uptime + '\n';
      txt += '  可用内存  ' + d.heap + '\n\n';
      txt += '👥 用户列表\n' + d.users + '\n';
      txt += '🔌 在线会话 (' + d.online + ')\n' + d.sessions + '\n';
      txt += '🎮 MAA 状态\n';
      txt += '  当前控制者  ' + d.activeUser + '\n';
      txt += '  开关状态   ' + (d.on ? '已开启' : '已关闭') + '\n\n';
      txt += '📬 最近通知 (' + d.webhookCount + ')\n';
      txt += '  ' + d.lastWebhookTitle + '\n';
      if (d.lastWebhookContent) txt += '  ' + d.lastWebhookContent + '\n';
      txt += '\n📋 最近日志\n' + d.logs;
      document.getElementById('adminInfo').textContent = txt;
    });
}

function checkLogin() {
  var token = getToken();
  var uname = getUsername();
  if (!token) { showView('loginView'); return; }
  fetch('/maa/state?token=' + encodeURIComponent(token))
    .then(function(r) {
      if (r.status === 200) {
        return r.json().then(function(d) {
          showView('maaView'); setConnState(true);
          if (d.username) {
            document.getElementById('welcomeUser').textContent = '👤 ' + d.username;
            setUsername(d.username);
          } else if (uname) {
            document.getElementById('welcomeUser').textContent = '👤 ' + uname;
          }
          if (d.maaUser) document.getElementById('maaUser').textContent = d.maaUser;
          if (d.isAdmin) document.getElementById('adminLink').classList.remove('hide-v');
          loadState();
          if (stateTimer) clearInterval(stateTimer);
          stateTimer = setInterval(loadState, 2000);
        });
      } else {
        clearToken(); showView('loginView');
      }
    })
    .catch(function() { showView('loginView'); });
}

window.addEventListener('load', function() {
  setTimeout(function() {
    document.getElementById('loader').classList.add('hide');
    document.body.classList.remove('is-preload');
    checkLogin();
  }, 1800);
});
</script>
</body>
</html>
)HTMLPAGE";
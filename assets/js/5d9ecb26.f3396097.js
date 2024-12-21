"use strict";(self.webpackChunkqtaskmachine_doc=self.webpackChunkqtaskmachine_doc||[]).push([[761],{4604:(e,t,n)=>{n.r(t),n.d(t,{assets:()=>o,contentTitle:()=>r,default:()=>l,frontMatter:()=>c,metadata:()=>a,toc:()=>d});const a=JSON.parse('{"id":"guides/basics/direct-state","title":"\u76f4\u63a5\u72b6\u6001","description":"\u76f4\u63a5\u987a\u5e8f\u6267\u884c\u7684\u72b6\u6001\uff0c\u4e0d\u4f1a\u7b49\u5f85\u4efb\u4f55\u4e8b\u4ef6\uff0centered\u4e0eexited\u4e8b\u4ef6\u4f1a\u7acb\u5373\u987a\u5e8f\u89e6\u53d1\u3002","source":"@site/docs/guides/basics/direct-state.md","sourceDirName":"guides/basics","slug":"/guides/basics/direct-state","permalink":"/qtaskmachine/docs/guides/basics/direct-state","draft":false,"unlisted":false,"tags":[],"version":"current","sidebarPosition":1,"frontMatter":{"sidebar_position":1},"sidebar":"guides","previous":{"title":"\u57fa\u7840\u72b6\u6001\u4f7f\u7528","permalink":"/qtaskmachine/docs/category/\u57fa\u7840\u72b6\u6001\u4f7f\u7528"},"next":{"title":"\u5ef6\u65f6\u72b6\u6001","permalink":"/qtaskmachine/docs/guides/basics/delay-state"}}');var s=n(4848),i=n(8453);const c={sidebar_position:1},r="\u76f4\u63a5\u72b6\u6001",o={},d=[];function u(e){const t={code:"code",h1:"h1",header:"header",p:"p",pre:"pre",...(0,i.R)(),...e.components};return(0,s.jsxs)(s.Fragment,{children:[(0,s.jsx)(t.header,{children:(0,s.jsx)(t.h1,{id:"\u76f4\u63a5\u72b6\u6001",children:"\u76f4\u63a5\u72b6\u6001"})}),"\n",(0,s.jsx)(t.p,{children:"\u76f4\u63a5\u987a\u5e8f\u6267\u884c\u7684\u72b6\u6001\uff0c\u4e0d\u4f1a\u7b49\u5f85\u4efb\u4f55\u4e8b\u4ef6\uff0centered\u4e0eexited\u4e8b\u4ef6\u4f1a\u7acb\u5373\u987a\u5e8f\u89e6\u53d1\u3002"}),"\n",(0,s.jsx)(t.p,{children:"\u4f7f\u7528\u793a\u4f8b\u5982\u4e0b\uff1a"}),"\n",(0,s.jsx)(t.pre,{children:(0,s.jsx)(t.code,{className:"language-cpp",children:'auto machine = new QStateMachine;\n//\u72b6\u6001\u5b9a\u4e49\nauto beginState = new DirectState(machine);\nbeginState->bindState(TaskStateType::State_Enter, machine, [&] {\n    qDebug() << "begin state run...";\n});\n\nauto secondState = new DirectState(machine);\nsecondState->bindState(TaskStateType::State_Enter, machine, [&] {\n    qDebug() << "second state run...";\n});\n\nauto finalState = new QFinalState(machine);\nconnect(finalState, &QState::entered, machine, [&] {\n    qDebug() << "final...";\n});\n\n//\u72b6\u6001\u5207\u6362\n*beginState >> secondState >> finalState;\n\n//\u521d\u59cb\u5316\u5e76\u5f00\u59cb\nmachine->setInitialState(beginState);\nmachine->start();\n'})}),"\n",(0,s.jsx)(t.p,{children:"\u6267\u884c\u4e0a\u9762\u7684\u4ee3\u7801\uff0c\u5c06\u4f9d\u6b21\u6253\u5370\uff1a"}),"\n",(0,s.jsx)(t.pre,{children:(0,s.jsx)(t.code,{className:"language-log",children:"[19:26:05.279] -> begin state run...\n[19:26:05.280] -> second state run...\n[19:26:05.280] -> final...\n"})})]})}function l(e={}){const{wrapper:t}={...(0,i.R)(),...e.components};return t?(0,s.jsx)(t,{...e,children:(0,s.jsx)(u,{...e})}):u(e)}},8453:(e,t,n)=>{n.d(t,{R:()=>c,x:()=>r});var a=n(6540);const s={},i=a.createContext(s);function c(e){const t=a.useContext(i);return a.useMemo((function(){return"function"==typeof e?e(t):{...t,...e}}),[t,e])}function r(e){let t;return t=e.disableParentContext?"function"==typeof e.components?e.components(s):e.components||s:c(e.components),a.createElement(i.Provider,{value:t},e.children)}}}]);
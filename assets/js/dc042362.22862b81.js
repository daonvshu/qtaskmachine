"use strict";(self.webpackChunkqtaskmachine_doc=self.webpackChunkqtaskmachine_doc||[]).push([[285],{1590:(e,n,t)=>{t.r(n),t.d(n,{assets:()=>o,contentTitle:()=>a,default:()=>u,frontMatter:()=>r,metadata:()=>s,toc:()=>d});const s=JSON.parse('{"id":"guides/functions/condition","title":"\u6761\u4ef6\u72b6\u6001\u5207\u6362","description":"\u6240\u6709\u57fa\u7840\u72b6\u6001\u53ef\u4ee5\u8bbe\u7f6e\u591a\u4e2a\u5207\u6362\u76ee\u6807\u5e76\u6309\u7167\u6761\u4ef6\u5207\u6362\uff0c\u4e00\u822c\u60c5\u51b5\u4e0b\u8c03\u7528>>\u8868\u793a\u5207\u6362\u5230\u4e0b\u4e00\u4e2a\u76ee\u6807\u72b6\u6001\uff0c\u800c\u9009\u62e9\u6027\u5207\u6362\u5230\u591a\u4e2a\u76ee\u6807\u4f7f\u7528<<\u6dfb\u52a0\u3002","source":"@site/docs/guides/functions/condition.md","sourceDirName":"guides/functions","slug":"/guides/functions/condition","permalink":"/qtaskmachine/docs/guides/functions/condition","draft":false,"unlisted":false,"tags":[],"version":"current","sidebarPosition":2,"frontMatter":{"sidebar_position":2},"sidebar":"guides","previous":{"title":"\u72b6\u6001\u5206\u7ec4","permalink":"/qtaskmachine/docs/guides/functions/group"},"next":{"title":"\u72b6\u6001\u6062\u590d","permalink":"/qtaskmachine/docs/guides/functions/recover"}}');var c=t(4848),i=t(8453);const r={sidebar_position:2},a="\u6761\u4ef6\u72b6\u6001\u5207\u6362",o={},d=[{value:"\u8bbe\u7f6e\u5207\u6362\u5206\u652f",id:"\u8bbe\u7f6e\u5207\u6362\u5206\u652f",level:2},{value:"\u8bbe\u7f6e\u5206\u652f\u6761\u4ef6",id:"\u8bbe\u7f6e\u5206\u652f\u6761\u4ef6",level:2}];function l(e){const n={code:"code",h1:"h1",h2:"h2",header:"header",p:"p",pre:"pre",strong:"strong",...(0,i.R)(),...e.components};return(0,c.jsxs)(c.Fragment,{children:[(0,c.jsx)(n.header,{children:(0,c.jsx)(n.h1,{id:"\u6761\u4ef6\u72b6\u6001\u5207\u6362",children:"\u6761\u4ef6\u72b6\u6001\u5207\u6362"})}),"\n",(0,c.jsxs)(n.p,{children:["\u6240\u6709\u57fa\u7840\u72b6\u6001\u53ef\u4ee5\u8bbe\u7f6e\u591a\u4e2a\u5207\u6362\u76ee\u6807\u5e76\u6309\u7167\u6761\u4ef6\u5207\u6362\uff0c\u4e00\u822c\u60c5\u51b5\u4e0b\u8c03\u7528",(0,c.jsx)(n.code,{children:">>"}),"\u8868\u793a\u5207\u6362\u5230\u4e0b\u4e00\u4e2a\u76ee\u6807\u72b6\u6001\uff0c\u800c\u9009\u62e9\u6027\u5207\u6362\u5230\u591a\u4e2a\u76ee\u6807\u4f7f\u7528",(0,c.jsx)(n.code,{children:"<<"}),"\u6dfb\u52a0\u3002"]}),"\n",(0,c.jsx)(n.h2,{id:"\u8bbe\u7f6e\u5207\u6362\u5206\u652f",children:"\u8bbe\u7f6e\u5207\u6362\u5206\u652f"}),"\n",(0,c.jsxs)(n.p,{children:["\u4f7f\u7528",(0,c.jsx)(n.code,{children:"<<"}),"\u9009\u62e9\u53ef\u4ee5\u5207\u6362\u7684\u4e0b\u4e00\u4e2a\u5206\u652f\uff1a"]}),"\n",(0,c.jsx)(n.pre,{children:(0,c.jsx)(n.code,{className:"language-cpp",children:"auto s1 = new DirectState;\nauto s2 = new DirectState;\nauto s3 = new DirectState;\nauto selectState = new DirectState;\nauto nextState = new DirectState;\n\n*selectState << s1 << s2 << s3 >> nextState;\n"})}),"\n",(0,c.jsxs)(n.p,{children:["\u8fd0\u7b97\u7b26",(0,c.jsx)(n.code,{children:"<<"}),"\u5c06\u4e0b\u4e2a\u5206\u652f\u6dfb\u52a0\u5230\u8be5\u5206\u652f\uff0c\u5e76\u4e14\u8fd4\u56de\u8be5\u5206\u652f\u5f15\u7528\uff0c\u5f53\u4e0b\u4e00\u6b21\u4f7f\u7528",(0,c.jsx)(n.code,{children:">>"}),"\u65f6\uff0c\u9690\u5f0f\u5305\u542b\u4ece\u5206\u652f\u72b6\u6001\u5207\u6362\u5230\u4e0b\u4e00\u4e2a\u5206\u652f\u3002\u4f8b\u5982\uff0c\u5982\u679c\u9009\u62e9\u5207\u6362\u5230",(0,c.jsx)(n.strong,{children:"s1"}),"\uff0c\u5219\u5305\u542b\u4e0b\u9762\u7684\u72b6\u6001\u8f6c\u79fb\u5173\u7cfb\uff1a"]}),"\n",(0,c.jsx)(n.pre,{children:(0,c.jsx)(n.code,{className:"language-cpp",children:"selectState >> s1 >> nextState;\n"})}),"\n",(0,c.jsxs)(n.p,{children:["\u8981\u5b9e\u73b0",(0,c.jsx)(n.code,{children:"A->B->C"}),"\u6216",(0,c.jsx)(n.code,{children:"A->C"}),"\u8fd9\u6837\u7684\u4e09\u89d2\u5207\u6362\uff0c\u9700\u8981\u5c06\u4e0b\u4e2a\u72b6\u6001\u4e5f\u6dfb\u52a0\u5230\u5206\u652f\u72b6\u6001\u4e2d\uff1a"]}),"\n",(0,c.jsx)(n.pre,{children:(0,c.jsx)(n.code,{className:"language-cpp",children:"*selectState << nextState << s1 >> nextState;\n"})}),"\n",(0,c.jsx)(n.h2,{id:"\u8bbe\u7f6e\u5206\u652f\u6761\u4ef6",children:"\u8bbe\u7f6e\u5206\u652f\u6761\u4ef6"}),"\n",(0,c.jsxs)(n.p,{children:["\u4f7f\u7528",(0,c.jsx)(n.code,{children:"setCondition"}),"\u51fd\u6570\u9009\u62e9\u5207\u6362\u7684\u5206\u652f\u7d22\u5f15\uff0c\u5728\u72b6\u6001\u8fdb\u5165",(0,c.jsx)(n.strong,{children:"enter"}),"\u4e4b\u524d\u8bbe\u7f6e\u751f\u6548\uff1a"]}),"\n",(0,c.jsx)(n.pre,{children:(0,c.jsx)(n.code,{className:"language-cpp",children:"auto s1 = new DirectState;\nauto s2 = new DirectState;\nauto s3 = new DirectState;\nauto selectState = new DirectState;\n\nauto beginState = new DirectState;\nbeginState->bindState(TaskStateType::State_Enter, this, [=] {\n    selectState->setCondition(0);\n});\n\nauto nextState = new DirectState;\n\n*beginState >> selectState << s1 << s2 << s3 >> nextState;\n"})}),"\n",(0,c.jsxs)(n.p,{children:[(0,c.jsx)(n.code,{children:"setCondition"}),"\u51fd\u6570\u4e5f\u53ef\u4ee5\u4f20\u9012\u4e00\u4e2alambda\u8868\u8fbe\u5f0f\uff0c\u5728\u81ea\u8eab\u8fdb\u5165",(0,c.jsx)(n.strong,{children:"enter"}),"\u65f6\u8c03\u7528\uff1a"]}),"\n",(0,c.jsx)(n.pre,{children:(0,c.jsx)(n.code,{className:"language-cpp",children:"selectState->setCondition([] {\n    return 0;\n});\n"})}),"\n",(0,c.jsxs)(n.p,{children:["\u8c03\u7528",(0,c.jsx)(n.code,{children:"setConditionDeferrable"}),"\u51fd\u6570\uff0c\u8ba9lambda\u8868\u8fbe\u5f0f\u5728\u8fdb\u5165",(0,c.jsx)(n.strong,{children:"exit"}),"\u65f6\u5ef6\u8fdf\u8c03\u7528\u8fdb\u884c\u9009\u62e9\uff1a"]}),"\n",(0,c.jsx)(n.pre,{children:(0,c.jsx)(n.code,{className:"language-cpp",children:"selectState->setConditionDeferrable();\n"})})]})}function u(e={}){const{wrapper:n}={...(0,i.R)(),...e.components};return n?(0,c.jsx)(n,{...e,children:(0,c.jsx)(l,{...e})}):l(e)}},8453:(e,n,t)=>{t.d(n,{R:()=>r,x:()=>a});var s=t(6540);const c={},i=s.createContext(c);function r(e){const n=s.useContext(i);return s.useMemo((function(){return"function"==typeof e?e(n):{...n,...e}}),[n,e])}function a(e){let n;return n=e.disableParentContext?"function"==typeof e.components?e.components(c):e.components||c:r(e.components),s.createElement(i.Provider,{value:n},e.children)}}}]);
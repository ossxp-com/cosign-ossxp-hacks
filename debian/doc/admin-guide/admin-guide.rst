================
单点登录
================

------------
管理员手册
------------

:作者: 北京群英汇信息技术有限公司
:网址: http://www.ossxp.com/
:版本: 3.1.1-1
:日期: |date|
:版权信息: Creative Commons

.. contents:: 目录
.. sectnum::
.. header:: 单点登录管理员手册
.. footer:: 北京群英汇信息技术有限公司
.. |date| date:: %Y-%m-%d %H:%M

群英汇单点登录系统是基于密歇根大学的开源项目 CoSign。群英汇为 CoSign 增加了本地化，使得 CoSign 能够同时支持两种甚至多种语言。群英汇还改造了 CoSign 的认证因子执行过程，增加了邮件地址登录，邀请码认证，admin 双因子认证，等等。

您可以参照群英汇的下列相关博客，了解 CoSign。

* `CoSign 2.x 协议介绍 <http://blog.ossxp.com/2010/03/824/>`_
* `CoSign 3.x 协议介绍 <http://blog.ossxp.com/2010/03/831/>`_
* 系列文章： `从 CoSign 看开源软件本地化 <http://blog.ossxp.com/2010/04/989/>`_
* 其他相关博客： http://blog.ossxp.com/tag/sso/

理解单点登录的认证过程
======================
CoSign 3.x 相比 CoSign 2.x 增加了两次跳转，导致概念上更加复杂。我们可以从 CoSign 2.x 入手了解单点登录认证过程。

CoSign 2.x 协议原理
-------------------
在 CoSign wiki 网站上，有两个协议实现的示例图，我们截取其中一个，用以说明。

   .. figure:: images/cosign-2-protocol.png
      :scale: 60
   
      CoSign 2.x 协议原理图

* 图示纵向三条线，代表了单点登录中涉及到的三个实体。
* 中间的一条线代表浏览器客户端，是 IE，FireFox, Google Chrome 之类的啦
* 左边的一条线代表Web应用
* 右边的一条线代表单点登录服务器（CGI+Daemon）
* 浏览器访问 Web 应用网站，若尚未登录，则：1. 由应用网站发放 Cookie（应用cookie）；2. 重定向到单点登录服务器的登录界面
* CoSign 2.x 的秘技：cookie 不是不能跨站么，CoSign 就通过 URL/Query string，将cookie的值 K/V 对传递到单点登录网站实现跨域；
* 在单点登录服务器的登录界面，由单点登录服务器签发cookie（登录 cookie）
* 登录成功后，CoSign Daemon 将登录cookie 和服务cookie两者做关联。
* 浏览器再次被重定向回 Web 应用网站。（浏览器会将 服务 cookie 传递给Web应用）
* Web应用所在的服务器，不会因为客户端发送了一个服务 cookie 就对客户端盲从，而是要通过专有协议（CoSign 协议, 6663 端口）和 CoSign Daemon 服务器连接，查询该服务器cookie的状态
* 如果经 CoSign Daemon 确认 服务 cookie 有效，则 Web 应用完成后续的登录注册工作;
* 如果 CoSign Daemon 返回了服务 cookie 无效的信息，则视同浏览器未登录，重新跳转到单点登录站点，开始新一轮的循环。

CoSign 2.x 的安全漏洞
---------------------
CoSign 2.x 版本存在一个安全风险，虽然不会影响服务器本身安全性，但存在被人利用，通过钓鱼手段获取权限的风险。CoSign 3.x 版本解决了这个问题。

CoSign 2.x 的这个安全风险源自于：

* 服务 cookie 由Web应用服务器发放，这时用户可能尚未登录
* 单点登录服务器建立登录 cookie 和服务 cookie 的关联非常随意，会被利用
* 黑客访问某个应用网帐，当重定向到单点登录服务器后，黑客的机器已经存在了相应的 服务 cookie，并且浏览器的地址栏会显示单点登录服务器跳转的 URL，并带有黑客自己的 cookie 值。
* 黑客只需要欺骗一个已经登录的用户点击这个URL，黑客就能够获取登录用户的权限，这是因为受害者已经通过点击伪装的 URL 对自己的 登录 cookie 和黑客的 服务 cookie 做了关联。

CoSign 3.x 认证原理
-------------------
CoSign 3.x 版本，在协议上进行了彻底的改造，解决了上面提到的 CoSign 2.x 的安全漏洞。

* 服务 Cookie 将不再由 Web 应用本身签发，而是在单点认证成功后，由单点登录服务器随机生成
* 单点登录随机生成的124位字串组成 ，通过 URL 重定向到 Web 应用所在服务器的特殊 url：/cosign/valid 进行校验，通过后，再设置服务 cookie。
* 跳转的 /cosign/valid 的 Web 地址，需要预先在单点登录服务器上配置好，不同的 服务 cookie 名称要有不同的 /cosign/valid/ 网址对应。
* 登录 cookie 和服务 cookie 的关联因此更为安全，因为服务 cookie 的随机数由单点登录服务器生成，而非应用本身
* 因此黑客在没有通过认证之前，无法得到 应用 cookie 值，也就无法通过“钓鱼”，来获得权限…

一个图片胜过千言万语。先来看看在官网上的示例图来学习 CoSign 3.x 协议原理：

   .. figure:: images/cosign-3-protocol.png
      :scale: 60
   
      CoSign 3.x 协议原理图

CoSign 3.x 流程和 CoSign 2.x 有很大的不同：

* 浏览器访问 Web 应用，将不再设置 Cookie。跳转到单点登录的 URL 中也没有 Cookie 值，只有一个孤孤零零的 cookie 名称
* 登录成功后，不但由单点登录服务器设置登录 cookie，而且还产生一个随机的 cookie 值，通过 query string 带给一个特殊的返回 URL
* 登录成功后跳转的 URL 不是跳转至单点登录服务器的原始 Web URL，而是在单点登录服务器针对 cookie 名称事先配置好的一个地址。这个地址的结尾一般是 /cosign/valid/
* 服务 cookie的签发是由 …./cosign/valid  的 URL 地址在验证完毕 cookie 值有效性后，再进行签发。之后在跳转到应用URL
* 注意：如果由于配置错误，/cosign/valid 所在的域名和应用 URL 不一致，将导致循环重定向错误


单点登录系统的部署
==================
CoSign 的组成模块
-----------------

CoSign 由以下几个模板组成：

* Daemon：服务器。实现 cosign 专有通讯协议，用于 登录cookie 和服务 cookie的关联
* CGI：登录界面。用于提供登录的 WEB 界面以及进行口令验证
* Factor：外挂的身份验证模块，用于实现口令认证或者其他方式的认证。例如 LDAP 认证，邀请码认证
* Filter：Web服务器挂件，安装在 Web 应用所在的 Web 服务器上，用于自动实现登录的重定向等

以上的模块除了 Factor 必须和 CGI 一同安装外，可以部署到不同的服务器中。

* Daemon 服务器可以和 CGI 部署在不同的服务器中，两者之间通过端口 6663 通讯
* Daemon 服务器和 CGI 都可以部署多个实例，以实现负载均衡
* Filter 作为 Apache（或者ISS）的插件，部署在 Web 应用服务器中，可以在任何服务器中部署
* Filter 和 CGI 服务器之间松耦合，实际上是通过 URL 重定向实现跳转
* Filter 和 Daemon 服务器之间通过运行在 6663 端口的协议进行通讯

有了上面的知识基础，作为管理员，应当知道在设置防火墙时，要允许 Web 应用服务器能够访问到单点登录的 Daemon 服务器的相应端口。

CoSign 各模板的安全证书
------------------------
CoSign 的各个模板之间的通讯是使用 SSL 加密的，而且要通过证书的 CN 字段的名称用于校验。

* 过滤器 Filter 要配置相应的安全证书来标识自己，如果 Filter 证书的 CN 名称在 CoSign Daemon 的配置文件中没有注册，则禁止和 CoSign Daemon 通讯
* CGI 也要配置相应证书来标识自己，如果 CGI 证书的 CN 名称在 CoSign Daemon 的配置文件中没有注册，则禁止和 CoSign Daemon 通讯
* CoSign Daemon 的证书的名称一般设置为该服务器的域名或主机名。如果 CGI 或 Filter 在连接 Daemon 时，发现 Daemon 证书的 CN 名称和连接时所用的域名/主机名不一致，则连接中断。

可以看出，CoSign 在设计中安全始终是放在第一位的。群英汇的单点登录软件包，内置了由群英汇签发的服务证书提供给 Daemon，CGI，filter使用，如果客户需要使用自签名证书，需要遵循下面的原则：

* 将您企业的 CA 根证书复制到 /opt/cosign/conf/certs/CA/ 目录下，并执行 make 命令重建 HASH
* 分别为 Daemon, CGI, Filter 创建证书，注意 CN 的名称非常重要
* 更改相关配置文件，用新的证书的 CN 名称替代原来配置中的证书 CN 名称。这些配置文件有：

  * 配置文件 /opt/cosign/conf/cosign.conf 中关于 CGI 证书位置，CoSign DAEMON 名称，CGI 和 Filter 证书CN 限制等配置
  * 配置文件 /etc/default/cosign 中关于 Daemon 证书位置
  * 配置文件 /etc/apache2/include/cosign/cosign-common.conf 中关于 Filter 证书位置，CoSign Daemon 主机名等
 
CoSign Daemon/CGI 的配置
-------------------------
参见 cosign.conf 手册。

CoSign Filter 的配置
------------------------

::

    CosignProtected               [ on | off ]
        governs whether Cosign is invoked or not
    
    CosignHostname                [ the name of the host running cosignd ]
    CosignValidReference          [ a regular expression matching valid
                                    service URLs. After the user logs into
                                    a service, they are redirected to the
                                    validation URL (e.g.,
                                    https://mail.example.edu/cosign/valid),
                                    where mod_cosign checks the user's cookie
                                    and compares the service URL to the value
                                    set by CosignValidReference. If the cookie
                                    and service URL are good, mod_cosign
                                    redirects to the service URL. ]
    CosignValidationErrorRedirect [ the URL to redirect to if the
                                    validation handler URL cannot
                                    validate the service cookie or the
                                    service URL. ]
    CosignRedirect                [ the URL of the cosign login cgi ]
    CosignPostErrorRedirect       [ the URL to redirect to if the user
                                    would be redirected to the login cgi
                                    during a POST. This screen lets people
                                    know we dropped their data. ]
    CosignService                 [ the name of the cosign service cookie ]
    CosignSiteEntry               [ the URL to redirect to after login  ]
    CosignCrypto                  [path to key] [path to cert] [path to CA dir]
    CosignRequireFactor           [ a list of the factors a user must satisfy ]
    CosignFactorSuffix            [ optional factor suffix when testing
                                    for compliance ]
    CosignFactorSuffixIgnore      [ on | off ]
    CosignHttpOnly                [ on | off ]
        module can be use without SSL - not recommended!
    CosignTicketPrefix            [ the path to the Kerberos ticket store ]
    CosignFilterDB                [ the path to the cosign filter DB]
    CosignFilterHashLength        [ 0 | 1 | 2 ]
        subdir hash for cosign filter DB
    CosignCheckIP                 [ never | initial | always ]
        check browser's IP against cosignd's ip information
    CosignProxyDB                 [ the path to the cosign proxy DB]
    CosignAllowPublicAccess       [ on | off ]
        make authentication optional for protected sites
    CosignGetKerberosTickets      [ on | off ]
        module asks for tgt from cosignd
    CosignKerberosSetupGSS        [ on | off ]
        setup the enviornment so that other apache modules
        that need GSSAPI/Kerberos work. e.g. IMP running under mod_php
    CosignGetProxyCookies         [ on | off ]
        module asks for proxy cookies from cosignd


如何定制单点登录界面模板
========================
CoSign 的用户界面是可以定制的，用户可以在单点登录界面中添加企业的 LOGO，可以在服务列表中添加其他整合到单点登录中的服务，可以添加其他语言的支持等等。

CoSign 的页面模板
-----------------
CoSign 的可定制性，源自于页面模板。CoSign 的页面模板文件位于目录 /opt/cosign/lib/templates-local 中。群英汇缺省提供两套界面模板，用户可以选择拷贝自己喜欢的模板到目录 /opt/cosign/lib/templates-local 中。

* CoSign 标准模板： 位于目录 /opt/cosign/lib/templates 中

   .. figure:: images/cosign-theme1.png
      :scale: 50
   
* Google 风格的模板： 位于目录 /opt/cosign/lib/templates_google 中

   .. figure:: images/cosign-theme2.png
      :scale: 50

关闭暂不使用的认证模式
----------------------
缺省提供了四种认证方式：1. 用户名口令认证，2. 一次性口令认证，3. 证书认证, 4. 邀请码认证。

其中第2种和第3种认证需要特殊的硬件设置或者特殊的服务器支持，一般来说应该关闭这两种认证模式，对于邀请码认证如果不需要，也可以关闭。

关闭不需要的认证模式很简单，需要管理员在CGI所在的服务器，编辑文件："/opt/cosign/lib/templates-local/inc/js_config.inc"

* 关闭一次性口令认证和证书认证，在 cosign.initUI() 之前添加一行，形如：

  ::
  
      cosign.setDisabledFactors('otp,kx509');
      cosign.initUI();

* 关闭一次性口令认证，证书认证和邀请码认证，在 cosign.initUI() 之前添加一行，形如：

  ::
  
      cosign.setDisabledFactors('otp,kx509,invite');
      cosign.initUI();

增加和删除单点登录关联服务
--------------------------
单点登录的界面会显示一个服务列表，这些服务列表用户可以根据需要增加和删除。定制也非常容易。

以中文界面为例，每个服务定义在一个文件中，内容实际为一个 html 片断，这些文件按照文件名顺序被单点登录页面包含。
在目录 /opt/cosign/lib/templates-local/inc/zh/ 中，可以看到缺省的服务列表文件：

::

  $ ls /opt/cosign/lib/templates-local/inc/zh/
  0000.html  0010_wiki.html  0015_blog.html  0020_scm.html  0030_bugtrac.html  0035_testlink.html  0040_comm.html  1000_account.html  9000.html

管理员在该目录中添加或者删除文件，就实现了对服务的定制。

注：英文服务包含文件在 /opt/cosign/lib/templates-local/inc/en/ 目录下。

如何配置邀请码认证
==================
邀请码认证是口令认证之外的另外一种认证方式。邀请码由管理员进行设置，为特殊用户提供无须用户注册即可访问某些服务。

在配置邀请码之前，首先需要通过用户管理系统添加邀请码对应的用户帐号：

* 该用户帐号不允许修改自己的口令，否则当取消邀请码认证后，仍会有人通过此帐号登录系统
* 该用户帐号的口令设置为一个复杂的口令，不需要记住的口令，因为该帐号通过邀请码登录而非口令认证
* 在各个应用系统中，为该帐号分配相应的权限

修改配置文件 "/opt/cosign/conf/config.py"，修改 InviteConfig 类中 maps 字典，将邀请码和用户帐号对应起来。

例如下的配置，将邀请码: abc321 对应于帐号 demo：

::

    class InviteConfig:
        maps = { 'demo': ['abc123'], }


* 可以为一个帐号设置多个邀请码
* 可以设置多个邀请码帐号，不同的邀请码对应不同的用户帐号

认证因子和 Admin 双因子认证
===========================
群英汇对 CoSign 双因子认证进行了改造，大大增强了单点登录系统的可用性和可配置性。

* 认证因子的反射机制，支持通过邮件名作为用户名进行认证
* 增加了新的双因子认证机制，能够使用和其他认证程序相同的认证字段
* 增加了认证因子的或运算，满足某些认证因子的任何一个即完成认证

认证因子的配置文件为 "/opt/cosign/conf/cosign.conf", 缺省的认证因子的配置为：

::

  factor /opt/cosign/factor/ldap2 login password
  factor /opt/cosign/factor/invite login invite
  factor /opt/cosign/factor/admin -3 login required

含义为：

* 如果提交的登录表单包含了 login 和 password 字段，则执行 ldap2 认证因子
* 如果提交的登录表单包含了 login 和 invite 字段，则执行 invite 认证因子
* 在完成了上述任何一个认证的情况下，并且登录表单包含 required 字段，则执行 admin 认证因子再次认证

通常情况下，只进行用户名口令认证（ldap2 认证因子），或者进行邀请码认证（invite 认证因子）。只有当认证请求中包含指定认证因子的认证，才会执行 admin 双因子认证。

下面以 OpenSesame 为例，介绍一个最简单的 admin 双因子认证：

* OpenSesame 是群英汇开发的动态打开端口的管理程序，通常用于向外部隐藏 SSH 登录端口，只有当访问特定 URL，通过认证之后，才向该 IP 地址动态打开特定端口。而且仅仅开放一段时间（5分钟），过时自动关闭端口。
* 访问 opensesame （芝麻开门）的 URL，如 https://weblogin.moon.ossxp.com/opensesame ， 会自动重定向到单点登录的 URL
  
  我们用 curl 命令连接该 URL，可以看到发生重定向 ::

    $ curl https://weblogin.moon.ossxp.com/opensesame
    <!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML 2.0//EN">
    <html><head>
    <title>302 Found</title>
    </head><body>
    <h1>Found</h1>
    <p>The document has moved <a href="https://weblogin.moon.ossxp.com/cgi-bin/login?factors=admin_ssh&amp;cosign-services&amp;https://weblogin.moon.ossxp.com/opensesame">here</a>.</p>
    </body></html>

* 浏览器跳转到单点登录。注意: URL 中包含 factors=admin_ssh， 即要求认证必须通过 admin_ssh 的认证因子请求
* Admin 认证因子会检查当前是否已经完成了用户名口令认证，然后检查数据库中该用户帐号是否包含相应的服务授权

  如果没有设置，单点登录检查名为 admin_ssh 的服务授权。而实际上，为了向下兼容，我们在配置文件中有如下配置：

  ::

    class LdapConfig:
        ...
        admin_auth_service_ssh = "ssh"

  即请求 admin_ssh 的认证因子，会检查名为 ssh 的服务授权。只要管理员为用户分配了名为 ssh 的服务授权，该用户就可以访问 opensesame。

* Apache 中的设置。参见文件 "/etc/apache2/include/opensesame/opensesame.conf"，其中的 CosignRequireFactor 指令自动在跳转 URL 中添加了 factors 参数

  ::

    <Directory /opt/ossxp/web/opensesame/>
      Options +FollowSymLinks

      <IfModule mod_cosign.c>
          # CoSign common settings if present.
          Include /etc/apache2/include/cosign/*.conf

          #CoSign Single Sign-on
          CosignProtected On
          # Cookie name:
          CosignService services
          CosignAllowPublicAccess Off

          AuthType Cosign
          CosignRequireFactor admin_ssh
      </IfModule>
      ...

可以在一次认证中同时对多个认证因子进行检查，支持认证因子的"与运算"和"或运算"。

* 访问单点登录的 URL 包含： factors=admin_a,admin_b,admin_c

  要求用户必须同时拥有 admin_a, admin_b 和 admin_c 的授权

* 访问单点登录的 URL 包含： factors=admin_a|admin_b,admin_c

  要求用户拥有 admin_a 或者 admin_b 的任何一种授权，并且同时拥有 admin_c 的授权

管理员可以通过命令行执行 admin 认证因子，来检查用户是否具有特定权限。例如执行下面的命令：

::

  $ /opt/cosign/factor/admin jiangxin admin_list,admin_ssh,admin_xyz
  admin_list,admin_ssh
  $ echo $?
  0

含义为： 

* 在命令行执行认证因子 admin
* 检查 jiangxin 用户，是否对 admin_list, admin_ssh 和 admin_xyz 拥有授权
* 认证因子的返回值为0，含义为认证成功
* 认证因子的字符串输出为 admin_list,admin_ssh，含义为: 只对 admin_list 和 admin_ssh 服务拥有授权

同样，管理员也可以手动执行 ldap2 和 invite 认证因子，诊断认证配置的正确性。

如何为新的服务配置单点登录认证
==============================
群英汇的单点登录软件包包含了一个演示网站： site1.foo.bar，开发人员可以参考其配置和代码，为其他应用开发单点登录支持。

* Apache 配置文件： /etc/apache2/sites-available/cosign-test1
* Web 网站示例脚本： /opt/cosign/sites/site1/www/


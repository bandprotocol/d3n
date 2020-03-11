module Styles = {
  open Css;

  let container =
    style([width(`percent(100.)), height(`percent(100.)), position(`relative)]);

  let innerContainer =
    style([
      maxWidth(`px(984)),
      marginLeft(`auto),
      marginRight(`auto),
      paddingLeft(Spacing.md),
      paddingRight(Spacing.md),
    ]);

  /* Main bar */
  let mainBar = style([display(`flex), paddingTop(Spacing.lg), cursor(`pointer)]);
  let version =
    style([
      display(`flex),
      borderRadius(`px(10)),
      backgroundColor(Colors.lightPink),
      padding4(~top=`px(0), ~bottom=`px(0), ~left=Spacing.sm, ~right=Spacing.sm),
      height(`px(20)),
      justifyContent(`center),
      alignItems(`center),
      marginLeft(Spacing.xs),
      marginTop(`px(1)),
    ]);

  let uFlex = style([display(`flex), flexDirection(`row)]);

  let bandLogo = style([width(`px(35))]);
  let twitterLogo = style([width(`px(20))]);
  let telegramLogo = style([width(`px(20))]);

  let skipRight = style([marginLeft(Spacing.xl)]);

  let socialLink =
    style([display(`flex), justifyContent(`center), alignItems(`center), width(`px(50))]);

  let logoContainer = style([display(`flex), alignItems(`center)]);

  let routeContainer = style([minHeight(`calc((`sub, `vh(100.), `px(300))))]);
};

[@react.component]
let make = () => {
  <div className=Styles.container>
    <NavBar />
    <div className=Styles.innerContainer>
      <div className=Styles.mainBar>
        <div className=Styles.logoContainer onClick={_ => Route.redirect(Route.HomePage)}>
          <Row>
            <Col size=1.> <img src=Images.bandLogo className=Styles.bandLogo /> </Col>
            <Col size=4.>
              <div className=Styles.uFlex>
                <Text value="D3N" size=Text.Xxxl weight=Text.Bold nowrap=true />
                <div className=Styles.version>
                  <Text value="v1.0 TESTNET" size=Text.Sm color=Colors.pink nowrap=true />
                </div>
              </div>
              <Text value="Data Request Explorer" nowrap=true />
            </Col>
          </Row>
        </div>
        <SearchBar />
        <div className=Styles.skipRight />
        <div className=Styles.socialLink>
          <a href="https://twitter.com/bandprotocol" target="_blank" rel="noopener">
            <img src=Images.twitterLogo className=Styles.twitterLogo />
          </a>
        </div>
        <div className=Styles.socialLink>
          <a href="https://t.me/bandprotocol" target="_blank" rel="noopener">
            <img src=Images.telegramLogo className=Styles.telegramLogo />
          </a>
        </div>
      </div>
      /* route handle */
      <div className=Styles.routeContainer>
        {switch (ReasonReactRouter.useUrl() |> Route.fromUrl) {
         | HomePage => <HomePage />
         | DataSourceHomePage => <DataSourceHomePage />
         | DataSourceIndexPage(dataSourceID, hashtag) =>
           <DataSourceIndexPage dataSourceID hashtag />
         | ScriptHomePage => <ScriptHomePage />
         | ScriptIndexPage(codeHash, hashtag) => <ScriptIndexPage codeHash hashtag />
         | TxHomePage => <TxHomePage />
         | TxIndexPage(txHash) => <TxIndexPage txHash />
         | BlockHomePage => <BlockHomePage />
         | BlockIndexPage(height) => <BlockIndexPage height />
         | ValidatorHomePage => <ValidatorHomePage />
         | RequestIndexPage(reqID, hashtag) => <RequestIndexPage reqID hashtag />
         | AccountIndexPage(address, hashtag) => <AccountIndexPage address hashtag />
         | ValidatorIndexPage(address) => <div> {"sdsdasfsaf" |> React.string} </div>
         | NotFound =>
           ReasonReactRouter.replace("/");
           React.null;
         }}
      </div>
    </div>
    <Footer />
  </div>;
};

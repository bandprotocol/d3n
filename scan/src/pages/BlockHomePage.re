module Styles = {
  open Css;

  let vFlex = align => style([display(`flex), flexDirection(`row), alignItems(align)]);

  let pageContainer = style([paddingTop(`px(50))]);

  let logo = style([width(`px(50)), marginRight(`px(10))]);

  let seperatedLine =
    style([
      width(`px(13)),
      height(`px(1)),
      marginLeft(`px(10)),
      marginRight(`px(10)),
      backgroundColor(Colors.gray7),
    ]);

  let fullWidth = style([width(`percent(100.0)), display(`flex)]);

  let withWidth = w => style([width(`px(w))]);

  let fillLeft = style([marginLeft(`auto)]);

  let icon =
    style([
      width(`px(30)),
      height(`px(30)),
      marginTop(`px(5)),
      marginLeft(Spacing.xl),
      marginRight(Spacing.xl),
    ]);
};

let renderBody = ((block, proposer): (BlockHook.Block.t, option(ValidatorHook.Validator.t))) => {
  let height = block.height;
  let timestamp = block.timestamp;
  let totalTx = block.numTxs;
  let hash = block.hash |> Hash.toHex(~upper=true);

  <TBody key={height |> string_of_int}>
    <Row minHeight={`px(40)}>
      <Col> <HSpacing size=Spacing.md /> </Col>
      <Col size=1.11> <TypeID.Block id={ID.Block.ID(height)} /> </Col>
      <Col size=3.93>
        <div className={Styles.withWidth(330)}>
          <Text value=hash weight=Text.Medium block=true code=true ellipsis=true />
        </div>
      </Col>
      <Col size=1.32> <TimeAgos time=timestamp size=Text.Md weight=Text.Medium /> </Col>
      <Col size=1.5>
        <div className={Styles.withWidth(150)}>
          {switch (proposer) {
           | Some(validator) => <ValidatorMonikerLink validator />
           | None => <Text value="Unknown" weight=Text.Medium block=true ellipsis=true />
           }}
        </div>
      </Col>
      <Col size=1.05>
        <Row>
          <div className=Styles.fillLeft />
          <Text value={totalTx |> Format.iPretty} code=true weight=Text.Medium />
        </Row>
      </Col>
      <Col> <HSpacing size=Spacing.md /> </Col>
    </Row>
  </TBody>;
};

[@react.component]
let make = () => {
  let (page, setPage) = React.useState(_ => 1);
  let limit = 10;

  let blocksOpt = BlockHook.latest(~limit, ~page, ());
  let infoOpt = React.useContext(GlobalContext.context);
  let blockCountOpt = BlockHook.latest();
  let numberOfPage =
    {
      let%Opt blocks = blockCountOpt;
      let%Opt latestBlock = blocks->Belt.List.get(0);
      Some(Page.findNumberOfPage(latestBlock.height, limit));
    }
    |> Belt.Option.getWithDefault(_, 1);

  let blocks = blocksOpt->Belt.Option.getWithDefault([]);

  let latestBlockOpt = blocks->Belt_List.get(0);
  let validators =
    switch (infoOpt) {
    | Some(info) => info.validators
    | None => []
    };

  let blocksWithProposers =
    blocks->Belt_List.map(block => (block, BlockHook.Block.getProposer(block, validators)));

  <div className=Styles.pageContainer>
    <Row>
      <Col>
        <div className={Styles.vFlex(`center)}>
          <img src=Images.blockLogo className=Styles.logo />
          <Text
            value="All BLOCKS"
            weight=Text.Medium
            size=Text.Md
            spacing={Text.Em(0.06)}
            height={Text.Px(15)}
            nowrap=true
            block=true
            color=Colors.gray7
          />
          <div className=Styles.seperatedLine />
          <Text
            value={
              switch (latestBlockOpt) {
              | Some(latestBlock) => latestBlock.height->Format.iPretty ++ " in total"
              | None => ""
              }
            }
            size=Text.Md
            weight=Text.Thin
            spacing={Text.Em(0.06)}
            color=Colors.gray7
            nowrap=true
          />
        </div>
      </Col>
    </Row>
    <VSpacing size=Spacing.xl />
    <THead>
      <Row>
        <Col> <HSpacing size=Spacing.md /> </Col>
        {[
           ("BLOCK", 1.11, false),
           ("BLOCK HASH", 3.93, false),
           ("AGE", 1.32, false),
           ("PROPOSER", 1.5, false),
           ("TXN", 1.05, true),
         ]
         ->Belt.List.map(((title, size, alignRight)) => {
             <Col size key=title justifyContent=Col.Start>
               <div className={Styles.vFlex(`flexEnd)}>
                 {alignRight ? <div className=Styles.fillLeft /> : React.null}
                 <Text
                   value=title
                   size=Text.Sm
                   weight=Text.Semibold
                   color=Colors.gray6
                   spacing={Text.Em(0.1)}
                 />
               </div>
             </Col>
           })
         ->Array.of_list
         ->React.array}
        <Col> <HSpacing size=Spacing.md /> </Col>
      </Row>
    </THead>
    {blocksWithProposers->Belt_List.toArray->Belt_Array.map(renderBody)->React.array}
    <VSpacing size=Spacing.lg />
    <Pagination currentPage=page numberOfPage onChangePage={newPage => setPage(_ => newPage)} />
    <VSpacing size=Spacing.lg />
  </div>;
};

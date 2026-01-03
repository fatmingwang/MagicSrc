Game Features（主畫面五大功能）等級與「整體升級」機制說明

概述
- 主畫面五個按鈕視為 5 個 Game Features：
  1. Gather（採集）
  2. Fight（戰鬥）
  3. Research（研究）
  4. Forge（鍛造）
  5. Fortify（防禦）

設計目標
- 讓玩家透過宏觀進度（Feature-level）提升整類單位的最大等級上限與能力成長空間，產生長期成就感與策略選擇。
- Feature 等級的提升由關卡選擇（Stage Select）畫面觸發（非戰鬥中直接升級），需滿足條件並消耗資源或特殊道具。

核心規則
1) Feature 等級上限
- 每個 Feature 有 0..5 的等級（共 6 個階段）。
- 初始 FeatureLevel = 0（未升級）；最高 FeatureLevel = 5。

2) 單位最大等級與 Feature 連動
- 單位基礎最高等級（BaseUnitMaxLevel）為 20（已在 experience data sheet 中定義）。
- 每提升 Feature 等級 +1，該 Feature 全部所屬單位最大等級上限增加 +10。
  - 例如：FeatureLevel = 1 → 單位最大等級 = 30；FeatureLevel = 5 → 單位最大等級 = 70。
- 當 FeatureLevel 提升後，所有該 Feature 下的現有單位都可繼續累積經驗至新的上限（不會自動提升等級，但可繼續獲 EXP 直到升級）。

3) Feature 升級觸發條件（Stage Select UI）
- 進入關卡選擇（Stage Select）時，系統檢查該 Feature 的「全域條件」是否滿足：
  A. 所有屬於該 Feature 的單位至少有一位已達當前單位最大等級上限（即已滿等）。或
  B. 或者達成替代條件（例如：完成指定任務、蒐集 N 個特殊素材、或完成研究項目）。
- 當條件滿足時，FeaturePanel（或 Stage Select）會顯示「可升級」提示，玩家可選擇以資源或道具消耗來執行 Feature 升級。

4) Feature 升級成本與冷卻
- 每次 Feature 升級都需消耗資源（可能為金錢、金屬、文化點或特殊憑證），成本隨等級遞增。可在資料表中調整數值。
- 升級為一次性（無需時間或可設定升級時間，例如 30s–120s）。

5) 風險與平衡
- 因為 Feature 升級會放大該類單位潛力，建議：
  - 升級成本較高且需玩家在關卡選單做出投資決策。
  - 每個玩家只能對其擁有的 Feature 執行升級。
  - 多人模式可能限制最大 FeatureLevel 或提高成本以維持平衡。

Feature 等級資料範例（說明）
- BaseUnitMaxLevel = 20
- UnitMaxIncreasePerFeatureLevel = 10
- FeatureLevelRange = 0..5

Feature 等級效果示例：
- FeatureLevel = 0 → UnitMax = 20
- FeatureLevel = 1 → UnitMax = 30
- FeatureLevel = 2 → UnitMax = 40
- FeatureLevel = 3 → UnitMax = 50
- FeatureLevel = 4 → UnitMax = 60
- FeatureLevel = 5 → UnitMax = 70

升級成本建議（可調整）
- 成本使用 4 類資源（Meat, Money, Metal, Wood）與一個特殊道具 `FeatureToken`（選用）：
  - Level 1 cost: {Meat:10, Money:200, Metal:50, Wood:50, FeatureToken:0}
  - Level 2 cost: {Meat:20, Money:400, Metal:120, Wood:100, FeatureToken:1}
  - Level 3 cost: {Meat:40, Money:900, Metal:300, Wood:240, FeatureToken:2}
  - Level 4 cost: {Meat:80, Money:2000, Metal:800, Wood:600, FeatureToken:4}
  - Level 5 cost: {Meat:200, Money:5000, Metal:2000, Wood:1500, FeatureToken:8}

使用流程（玩家角度）
1. 在遊戲中讓該 Feature 的多數單位達到當前上限（例如把多個農夫練到 L20）。
2. 進入關卡選擇畫面（Stage Select），系統提示某 Feature 可升級。
3. 玩家檢視升級效果（新 UnitMax、額外解鎖）、確認並支付成本。
4. 系統提升 FeatureLevel，更新該 Feature 所有單位的最大等級上限（例如從 20→30）。
5. 玩家可在後續遊玩中繼續讓單位升等至新上限。

資料與 API 建議
- 前端需要的欄位（Feature 物件）：
  - id, name, description_zh, currentLevel, maxLevel, baseUnitMax, unitIncreasePerLevel, nextLevelCost (資源結構), unlocks
- 後端 API 範例：
  - GET /api/features → 回傳所有 Feature 的目前狀態
  - POST /api/features/{id}/upgrade → 嘗試升級（回傳 success / fail 與訊息）

後續建議
- 若同意此設計，我可生成：
  1) `Doc/data/game_features.json`（Feature 的預設資料與各級成本），
  2) 前述 API 的 JSON schema 範例，或
  3) 在 `Doc/data/units_level_table.json` 反映 Feature 等級上限影響（自動生成不同 FeatureLevel 下的單位等級上限）。

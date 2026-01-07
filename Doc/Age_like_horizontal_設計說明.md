# 專案名稱（工作名稱）
- 工作名稱：類《世紀帝國》風格（水平向）— 以單向軸向進行的基地經營與自動戰鬥混合遊戲

## 高層概念
- 受《世紀帝國》啟發的基地建設與資源管理，但戰鬥限制在單一方向（水平或垂直）。
- 戰鬥為自動化（無直接單位微操），在攻擊模式提供有限互動與組合技能，結合放置與策略要素(但可以放技能Line Ranger)。

## 核心特性
- 單軸戰鬥：戰鬥沿一條固定軸線展開，無完整 2D 操控。
- 單位自動控制：單位由 AI 驅動，玩家管理部隊組成、升級、裝備與基地佈局。
- 攻擊模式互動：攻擊期間可觸發組合技與有限操作。
- 多人陣型（DLC）：以正多邊形頂點配置玩家並衍生多條攻擊路徑。

## 資源
- 建議基本資源（4–8 類）：
  - 肉（食物）
  - 金錢（貨幣）
  - 金屬
  - 木材
- 可擴充：石料、文化點、燃料等。
- 玩家要自己控制先採取什麼資源,兵種不同需要的資源也不同,資源點也能升級,被採取光了需要一段時間才會再生,或是要找新的資源點

## 基地介面 / 互動
- 主畫面有 5 個主要按鈕，每按鈕開啟浮動視窗（Modal）：
  1. Gather（採集）/ 工人相關：採集資源、操作生產建築與駐守防禦武器。
  2. Fight（戰鬥）/ 軍隊：招募士兵，用於進攻或防守。
  3. Research（研究）/ 學校：提升工人/士兵等級，強化工廠與防禦效率。
  4. Forge（鍛造）/ 工具工廠：生產裝備與防禦武器。
  5. Fortify（防禦）/ 基地防禦：建置武器，部分需人員操作以達最大效能。
- 浮動視窗(tabpage or split window)可檢視、排隊、生產、升級、分配與製造。

## 成長與解鎖
- 升級（玩家或基地）可擴展基地區域並開放更多建築格位。
- 更高等級解鎖新單位、建築等級與組合技能。
- 升級時會有選擇A,B,C方向會導致可以產生的部隊會有所不同

## 戰鬥設計要點
- 單位自動前進並沿軸線交戰，由系統結算。
- 玩家無法微操單位，但管理編成、裝備與在攻擊模式觸發組合技。
- 地形有丘陵森林等，影響視野與行進速度。
- 組合技範例：
  - 3 騎士 + 2 步兵 → 近戰合擊
  - 1 投石機 + 2 弓兵 → 多重投擲
- 組合技具成員需求、冷卻與資源消耗。

## DLC：多人模式（付費擴充）
- 概要：多人模式作為 DLC 發行，包含線上配對、房間、排行與多人專屬地圖。購買 DLC 後可開啟「正多邊形攻擊模式」。
- 功能：
  - 線上配對與私人房間、賽季排行與獎勵、多人平衡調整。
  - 正多邊形攻擊模式（玩法說明）
  - 概念：對戰場域以正 n 邊形表示，玩家各自站在頂點（n 為玩家數）。戰鬥沿從頂點延伸的攻擊路徑展開，路徑可為邊或對角線，視模式而定。
  
- 流程：
  1. 選擇玩家數（支援 2 至 上限）。
  2. 系統生成正多邊形並分配玩家至頂點。
  3. 玩家在基地準備部隊/裝備並選擇要派出的路徑（或由系統自動）。
  4. 戰鬥自動進行：單位沿路徑前進並在相遇處交戰；路徑交會處可產生交互效果（群體傷害、組合觸發）。

- 路徑模式示例：
  - 只邊模式（Neighbor Mode）：僅允許相鄰頂點間的邊作為攻擊路徑。
  - 全連接模式（All-Connect Mode）：允許所有頂點間連線（含對角線）。
  - 自訂模式：房主或規則可限制可用連線（例：僅允許跳過 1 個頂點的連線）。

- 範例說明：
  - 2 人：等同水平直線對抗（A ↔ B）。
  - 3 人：等邊三角形，邊為主要路徑。 
  - 4 人：正方形，選擇僅邊或含對角線（交叉點為高衝突區）。
  - 5 人：正五邊形，包含邊與對角線，會出現五角星型攻擊路徑。

- 戰術與平衡：
  - 路徑決定目標分配與聚集位置，影響隊伍搭配。
  - 交會點為熱點，可設計為觸發聯合組合或提供資源獎勵。
  - 組合技可延伸為跨路徑或交會點強化。
  - 多人模式需嚴格限制人口與生產速率以維持平衡與效能。

## 視覺示意圖
- 建議加入示意圖展示玩家間攻擊路徑：
  - `Doc/images/attack_2.svg`：2 人對戰（直線）。
  - `Doc/images/attack_3.svg`：3 人對戰（三角形）。
  - `Doc/images/attack_4.svg`：4 人對戰（正方形含對角線）。
  - `Doc/images/attack_5.svg`：5 人對戰（五角星連線示意）。

## 未決議題
- 水平或垂直方向是否可於每場切換？
- 資源種類與數量（4–8 類）最終決定。
- 人口上限與成長曲線數值。
- 組合系統的具體公式、成本與冷卻設定。
    - 多人陣型的詳細平衡規則與地圖設計。

## 建議後續步驟
1. 決定資源清單與單位表。
2. 繪製 5 個浮動視窗的 UI 線框並列出每視窗操作。
3. 制定組合技初版規則與平衡草案。
4. 製作單軸戰鬥原型以測試節奏與組合感受。
5. 設計多人匹配規則與陣型選擇界面。

## 五個主要介面功能詳述
下面為主畫面五個按鈕各自對應的功能面板（浮動視窗）與常見按鈕、行為、範例資源消耗與限制。可作為 UI 與後端事件設計的初版規格。

1) Gather（採集）
- 目的：管理人口、資源採集與基地內工作分配。
- 面板顯示：總工人数、空閒工人、資源採集任務、工人等級、可指派建築。
- 常用按鈕與功能：
  - `招募農夫（Recruit Farmer）`：消耗肉 + 金錢（範例：10 肉，5 金），招募一位專職採集食物的工人，需時間 10s。
  - `招募漁夫（Recruit Fisherman）`：消耗肉 + 金錢（範例：8 肉，6 金），可在水域加速食物產出，需時間 12s。
  - `指派採集（Assign Gather）`：將空閒工人分配到某資源（木/金屬/肉/金錢），立即生效。
  - `自動採集切換（Auto-Collect Toggle）`：切換該資源是否允許自動派工（ON/OFF）。
  - `Gather 升級（Upgrade Gather）`：消耗金屬/金錢，提升所有工人的採集效率或移動速度（有冷卻）。
- 限制與備註：人口上限影響招募；部分工作（建築維修、駐守武器）需要指定工人駐守。

2) Fight（戰鬥）
- 目的：招募與管理戰鬥部隊、配置出征或防守編成。
- 面板顯示：可用軍隊數、隊列、隊伍組合、士兵等級、戰鬥技能（combo）提示。
- 常用按鈕與功能：
  - `招募騎士（Hire Knight）`：消耗金錢 + 金屬（範例：20 金、10 金屬），生產時間 20s，較高近戰傷害與移動速率。
  - `招募弓手（Hire Archer）`：消耗金錢 + 木材（範例：15 金、6 木），生產時間 15s，遠程攻擊。
  - `快速招募（Quick Recruit xN）`：以較高金錢花費立即完成多位招募（排隊購買）並加冷卻或溢價費用。
  - `派出偵查（Dispatch Scout）`：派出偵查小隊可短時間查看敵路徑/視野（消耗少量金錢）。
  - `設定出征路徑（Set Attack Path）`：選擇在對戰中要使用的攻擊路徑（單人模式為主要軸；多人為多邊形路徑）。
- 組合技觸發提示：面板會顯示可用的組合技（ex. 3 騎士 + 2 步兵），玩家可在攻擊模式手動觸發或設定自動條件。

3) Research（研究）
- 目的：研究與訓練，提升人口與軍隊的基礎能力、解鎖技能與科技。
- 面板顯示：可研究項目、研究隊列、當前等級、研究點或資源消耗顯示。
- 常用按鈕與功能：
  - `研究：工人效率（Research: Worker Efficiency）`：消耗金錢 + 文化點（範例：50 金、10 文化），增加所有工人採集速率，研究時間 60s。
  - `研究：軍隊攻擊（Research: Army Offense）`：提升特定部隊攻擊力，消耗金錢+金屬。
  - `訓練士兵（Train Soldiers）`：消耗資源並在時間內提升已有士兵等級或經驗（短期增益）。
  - `解鎖組合技（Unlock Combo）`：研究特定組合技能（需達到部隊或學校等級門檻）。
- 備註：Research 可設計為影響整體科技樹，並與文化或時代系統連動。

4) Forge（鍛造）
- 目的：生產工具、裝備與防禦武器零件，供工人與士兵使用提升效能。
- 面板顯示：生產佇列、現有材料、成品庫存、工廠等級與產能。
- 常用按鈕與功能：
  - `生產工具（Produce Tool）`：消耗木材/金屬（範例：5 木、3 金屬），生產一個工具以提升工人採集速度或解鎖作業。
  - `製造武器（Produce Weapon）`：消耗金屬 + 金錢，產出供士兵裝備的武器，提升其戰鬥數值。
  - `批量生產（Batch Produce xN）`：一次下多單，節省每件成本或延長生產時間。
  - `維修與保養（Repair / Maintain）`：使用材料修復基地防禦或減少防禦武器故障概率。
- 備註：Forge 可有生產效率層級，升級可降低材料消耗或縮短生產時間。

5) Fortify（防禦）
- 目的：建造、部署與管理基地防禦設施，並分配駐守人員以提高防守能力。
- 面板顯示：已建防禦設施列表、可建設項目、駐守人數、耐久與操作狀態。
- 常用按鈕與功能：
  - `建造箭塔（Build Arrow Tower）`：消耗木材 + 金屬（範例：30 木、15 金屬），在指定格位建造箭塔，需工人或士兵駐守啟用全效能。
  - `建造投石機（Build Catapult）`：消耗金屬 + 金錢，長射程群體傷害武器，需士兵操作以裝填砲彈並發射。
  - `部署陷阱（Deploy Trap）`：消耗材料放置地面陷阱或減速區域，觸發型消耗低但不可直接操作。
  - `分配駐守（Assign Crew）`：將工人或士兵指派至特定防禦項目（例如箭塔、投石機），無人時效能降低。
  - `升級防禦（Upgrade Defense）`：消耗資源提升該建築耐久/攻擊力或降低冷卻時間。
- 備註：部分防禦裝置需要工具工廠生產的特殊彈藥或配件才能啟用。

## 遊戲主要階段（Game Phases）
遊戲分為三個主要階段：標題畫面、關卡選擇（含科技樹預覽）與遊戲進行階段。以下為各階段功能描述與 UI 建議。

1) 標題畫面（Title / Loading）
- 功能：載入資源、顯示遊戲標題與選單（新遊戲、繼續、設定、離開）。
- 流程與要點：
  - 啟動時進入載入流程（Loading）。載入期間循環顯示數張遊戲截圖或背景動畫、進度條與提示文字。
  - 載入完成後顯示主選單：`新遊戲（New Game）`、`繼續（Continue）`（若有存檔）、`設定（Options）`、`多人（Multiplayer / DLC）`、`離開（Quit）`。
  - 新遊戲進入關卡選擇階段；繼續則讀取最近存檔並回到關卡選單或遊戲內。
- UI 建議：背景淡入主題音樂，截圖支援滑動或淡入淡出切換；提供快速鍵與無障礙提示。

2) 關卡選擇與科技預覽（Stage Select & Tech Preview）
- 功能：玩家選擇關卡/地圖，並預覽該關卡可用科技樹、解鎖單位與資源特性。
- 面板與要素：
  - 關卡列表：顯示名稱、難度、推薦等級與通關獎勵。
  - 科技樹預覽：展示該關卡可研究或可用的科技分支（可點擊查看詳細消耗與效果）。
  - 可選參數：遊戲模式（單人、DLC 多人）、地圖方向（水平/垂直）、初始資源包或限制（挑戰模式）。
  - 關卡資訊：包含敵方勢力描述、地形摘要（是否有水域、礦脈密度）與時間限制／勝利條件。
- 流程與按鈕：
  - `檢視科技（View Tech）`：彈出科技樹詳情。
  - `選擇關卡（Select Stage）`：進入預備畫面（佈署與確認）。
  - `返回（Back）`：回到主選單。
- 平衡提示：關卡可設定不同資源豐富度與人口上限以增加變化性。

3) 遊戲內容階段（Gameplay）
- 功能：玩家在基地收集資源、建設、訓練與指派部隊，運用策略擊敗對手或完成關卡目標。
- 主要系統互動：
  - 資源採集：透過 `工人` 面板指派採集或開啟自動採集。
  - 生產與訓練：使用 `工具工廠` 與 `軍隊` 面板排隊生產工具與士兵。
  - 防禦佈署：在 `基地防禦` 面板建造與分配駐守。
  - 科技與升級：在 `學校` 面板啟動研究，影響長期效率與組合技。
  - 出征機制：在攻擊模式或多邊形多人模式透過 `設定出征路徑` 指定隊伍行進路徑，或設為自動依陣型派遣。
- 勝利條件（範例）：摧毀主要敵方建築、佔領指定據點或在限定時間內保護要塞存活。
- UI / UX 要點：
  - 提供暫停與快速存檔功能。
  - 攻擊模式顯示即時組合技按鈕與冷卻提示，並在單位進場時顯示路徑與預估交戰點。
  - HUD 顯示當前資源、人口與目標，並能快速呼出任一主要面板（5 大按鈕）。

---

說明：上述為三大階段的初版設計，包含建議 UI 元素與流程。若需要，我可以把每個階段的 UI 以 JSON 規格描述（包含按鈕 ID、事件與回傳參數），或生成原型畫面線框圖素材（SVG/PNG）。

## 遊戲物件基本屬性（Game Object Attributes）
以下為所有遊戲物件（單位、建築、裝備）應具備的基本屬性定義與範例。屬性以簡單欄位呈現，`behavior` 使用 JSON 格式描述 AI 可行為或限制，方便序列化存檔與網路傳輸。

- 共用屬性說明：
  - `name`：字串，物件名稱（英）。
  - `description_zh`：字串，中文描述（簡短，供 UI 顯示）。
  - `type`：字串，物件類別（例如 `worker`、`soldier`、`building`、`equipment`）。
  - `speed`：數值，移動速度（單位：格/秒 或 單位/秒）。建築此值為 0。
  - `attack`：物件攻擊描述，數值或物件。範例可為整數（近戰傷害）或物件：`{"damage":10,"rate":1.2,"range":[1,3]}`。
  - `HP`：整數，生命值／耐久。
  - `thinking_idle_time`：數值，AI 間隔再評估行為的秒數（空閒思考時間）。
  - `behavior`：JSON，描述物件可執行行為、限制與偏好（見範例）。
  - `level`：整數，當前等級（預設 1）。
  - `exp`：數值，當前經驗值（用於升級判定）。
  - `levelUpRules`：JSON，描述升級需求與升級獲得（詳見下方範例）。

- 說明：`levelUpRules` 應包含以下欄位：
  - `expToNext`：升級所需經驗值（或描述型需求）。
  - `expSource`：描述如何獲得經驗（例如每 X 秒從事某任務或每次擊殺獲得經驗）。
  - `onLevelUp`：升級時獲得的屬性變更（可用百分比或固定值）。

- 範例：工人類（Workers）
  - 農夫（Farmer）:

```json
{
  "name": "Farmer",
  "description_zh": "專職耕作的工人，擅長食物採集（田地）",
  "type": "worker",
  "speed": 1.0,
  "attack": {"damage": 1, "rate": 1.0, "range": [1,1]},
  "HP": 50,
  "thinking_idle_time": 3.0,
  "behavior": {
    "allowTasks": ["farm","gather_food"],
    "resourceFilter": ["meat"],
    "priority": ["farm","deliver"],
    "canRepair": false,
    "requiresTool": false
  },
  "level": 1,
  "exp": 0,
  "levelUpRules": {
    "expToNext": 60,
    "expSource": "每 5 秒持續耕作獲得 1 EXP（等效 5 分鐘 = 300 秒 可達 60 EXP）",
    "onLevelUp": {"gatherRatePercent": 10, "HPPlus": 5, "note": "提高採集速率與少量生命值"}
  }
}
```

  - 漁夫（Fisherman）：

```json
{
  "name": "Fisherman",
  "description_zh": "在水域採集食物的工人，需水域才能工作",
  "type": "worker",
  "speed": 0.9,
  "attack": {"damage": 1, "rate": 1.0, "range": [1,1]},
  "HP": 45,
  "thinking_idle_time": 4.0,
  "behavior": {
    "allowTasks": ["fish","gather_food"],
    "requiredTerrain": ["water"],
    "priority": ["fish","deliver"]
  },
  "level": 1,
  "exp": 0,
  "levelUpRules": {
    "expToNext": 50,
    "expSource": "每 6 秒持續垂釣／採集水域資源獲得 1 EXP（約 5 分鐘可達 50 EXP）",
    "onLevelUp": {"gatherRatePercent": 12, "HPPlus": 4, "note": "提升漁獲效率"}
  }
}
```

  - 採礦工（Miner）：

```json
{
  "name": "Miner",
  "description_zh": "採掘礦石與石料的工人，能操作採礦相關機具",
  "type": "worker",
  "speed": 0.8,
  "attack": {"damage": 2, "rate": 1.0, "range": [1,1]},
  "HP": 55,
  "thinking_idle_time": 5.0,
  "behavior": {
    "allowTasks": ["mine","dig_stone","dig_gold"],
    "resourceFilter": ["metal","stone"],
    "priority": ["mine","deliver"],
    "canOperateMachinery": true
  },
  "level": 1,
  "exp": 0,
  "levelUpRules": {
    "expToNext": 60,
    "expSource": "每 5 秒持續採礦獲得 1 EXP",
    "onLevelUp": {"gatherRatePercent": 15, "HPPlus": 6, "note": "採礦效率與耐久提升"}
  }
}
```

- 範例：軍隊（Army）
  - 騎士（Knight）:

```json
{
  "name": "Knight",
  "description_zh": "重裝近戰單位，耐久與近戰傷害較高",
  "type": "soldier",
  "speed": 1.6,
  "attack": {"damage": 18, "rate": 1.0, "range": [1,1]},
  "HP": 200,
  "thinking_idle_time": 1.0,
  "behavior": {
    "role": "melee",
    "engageRange": [1,2],
    "preferTarget": ["archer","worker"],
    "formationPosition": "front"
  },
  "level": 1,
  "exp": 0,
  "levelUpRules": {
    "expToNext": 100,
    "expSource": "每擊殺敵方單位獲得 20 EXP；受傷或造成大量傷害亦可獲少量 EXP",
    "onLevelUp": {"attackPercent": 8, "HPPercent": 10, "speedPercent": 2, "note": "近戰傷害與耐久提升"}
  }
}
```

  - 弓手（Archer）:

```json
{
  "name": "Archer",
  "description_zh": "遠程單位，具有最小與最大射程限制，適合在後方輸出",
  "type": "soldier",
  "speed": 1.1,
  "attack": {"damage": 9, "rate": 1.2, "range": [3,8]},
  "HP": 80,
  "thinking_idle_time": 1.5,
  "behavior": {
    "role": "ranged",
    "minRange": 3,
    "maxRange": 8,
    "preferTarget": ["cavalry","melee"],
    "holdPosition": true
  },
  "level": 1,
  "exp": 0,
  "levelUpRules": {
    "expToNext": 90,
    "expSource": "每擊中敵方單位獲得 10 EXP；命中越多累積越快",
    "onLevelUp": {"attackPercent": 10, "maxRangePlus": 1, "HPPlus": 6, "note": "射程或攻擊力微幅提升"}
  }
}
```

  - 偵查（Scout）：（低 HP、高速）

```json
{
  "name": "Scout",
  "description_zh": "快速偵查單位，優先發現敵情並避免交戰",
  "type": "soldier",
  "speed": 2.4,
  "attack": {"damage": 4, "rate": 1.0, "range": [1,1]},
  "HP": 40,
  "thinking_idle_time": 0.8,
  "behavior": {
    "role": "scout",
    "avoidCombat": true,
    "scoutRadius": 8
  },
  "level": 1,
  "exp": 0,
  "levelUpRules": {
    "expToNext": 50,
    "expSource": "每 20 秒持續偵查／探索獲得 1 EXP；或發現敵方目標獲得額外 EXP",
    "onLevelUp": {"speedPercent": 8, "scoutRadiusPlus": 1, "HPPlus": 3, "note": "偵查範圍與速度提升"}
  }
}
```

- 範例：學校（School）與訓練相關物件
  - 學校（building）:

```json
{
  "name": "School",
  "description_zh": "研究與訓練中心，可進行科技研究與解鎖升級",
  "type": "building",
  "speed": 0,
  "attack": null,
  "HP": 600,
  "thinking_idle_time": 10.0,
  "behavior": {
    "canResearch": true,
    "researchSpeed": 1.0,
    "availableUpgrades": ["worker_efficiency","army_offense"]
  },
  "level": 1,
  "exp": 0,
  "levelUpRules": {
    "expToNext": 0,
    "expSource": "建築等級由資源升級（非經驗）控制",
    "onLevelUp": {"researchSpeedPercent": 15, "availableSlotsPlus": 1, "note": "提高研究速度並開放更多隊列"}
  }
}
```

  - 訓練師（Trainer，NPC）:

```json
{
  "name": "Trainer",
  "description_zh": "在學校內負責訓練單位，提升經驗獲得或短期增益",
  "type": "worker",
  "speed": 1.0,
  "attack": {"damage":2,"rate":1.0,"range":[1,1]},
  "HP": 70,
  "thinking_idle_time": 6.0,
  "behavior": {
    "role": "train",
    "canTrain": ["soldier","worker"],
    "bonusExpRate": 1.2
  },
  "level": 1,
  "exp": 0,
  "levelUpRules": {
    "expToNext": 80,
    "expSource": "透過訓練任務獲得 EXP；每完成 N 次訓練給予 X EXP",
    "onLevelUp": {"bonusExpRatePercent": 10, "HPPlus": 5, "note": "提升訓練加成"}
  }
}
```

- 範例：工具工廠（Tool Factory）與裝備
  - 工具（Tool）：（通常不升級）

```json
{
  "name": "Basic Pickaxe",
  "description_zh": "基礎採礦工具，增加採礦效率",
  "type": "equipment",
  "speed": 0,
  "attack": null,
  "HP": null,
  "thinking_idle_time": 0,
  "behavior": {
    "equipFor": ["worker"],
    "gatherBonus": {"metal": 1.5}
  },
  "level": null,
  "exp": null,
  "levelUpRules": null
}
```

  - 武器（Weapon）：

```json
{
  "name": "Iron Bow",
  "description_zh": "基礎弓具，提供弓手使用以增加遠程傷害",
  "type": "equipment",
  "attack": {"damage": 5, "rate": 1.2, "range": [3,7]},
  "behavior": {"equipFor": ["Archer"]},
  "level": null,
  "exp": null,
  "levelUpRules": null
}
```

- 範例：基地防禦（Base Defending）
  - 箭塔（Arrow Tower）：

```json
{
  "name": "Arrow Tower",
  "description_zh": "自動防禦建築，提供範圍內遠程打擊",
  "type": "building",
  "speed": 0,
  "attack": {"damage": 12, "rate": 0.8, "range": [2,10]},
  "HP": 800,
  "thinking_idle_time": 2.0,
  "behavior": {
    "requiresCrew": 1,
    "crewType": ["worker","soldier"],
    "targetPriority": ["infantry","cavalry","scout"]
  },
  "level": 1,
  "exp": 0,
  "levelUpRules": {
    "expToNext": 0,
    "expSource": "防禦建築升級以資源為主；若綁定駐守人可有額外經驗值增長",
    "onLevelUp": {"attackPercent": 12, "HPPercent": 15, "note": "透過升級強化火力與耐久"}
  }
}
```

  - 投石機（Catapult）：

```json
{
  "name": "Catapult",
  "description_zh": "投射大範圍彈藥的攻城武器，需多名駐守與彈藥供給",
  "type": "building",
  "speed": 0,
  "attack": {"damage": 60, "rate": 0.2, "range": [6,20]},
  "HP": 1200,
  "thinking_idle_time": 5.0,
  "behavior": {
    "requiresCrew": 2,
    "ammunitionType": "stone",
    "areaEffectRadius": 3
  },
  "level": 1,
  "exp": 0,
  "levelUpRules": {
    "expToNext": 0,
    "expSource": "同上以資源升級為主",
    "onLevelUp": {"attackPercent": 20, "areaEffectRadiusPlus": 1, "note": "提升投射威力與範圍"}
  }
}
```

---

說明：上述為初步設計文件，後續可依需求調整與細化，包含資源種類、科技樹、單位與建築物件屬性等。如需進一步討論或修改，請提出具體需求與方向。